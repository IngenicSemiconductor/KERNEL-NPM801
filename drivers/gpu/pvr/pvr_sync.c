/*************************************************************************/ /*!
@File           pvr_sync.c
@Title          Kernel sync driver
@Copyright      Copyright (c) Imagination Technologies Ltd. All Rights Reserved
@Description    Version numbers and strings for PVR Consumer services
				components.
@License        Dual MIT/GPLv2

The contents of this file are subject to the MIT license as set out below.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

Alternatively, the contents of this file may be used under the terms of
the GNU General Public License Version 2 ("GPL") in which case the provisions
of GPL are applicable instead of those above.

If you wish to allow use of your version of this file only under the terms of
GPL, and not to allow others to use your version of this file under the terms
of the MIT license, indicate your decision by deleting the provisions above
and replace them with the notice and other provisions required by GPL as set
out in the file called "GPL-COPYING" included in this distribution. If you do
not delete the provisions above, a recipient may use your version of this file
under the terms of either the MIT license or GPL.

This License is also included in this distribution in the file called
"MIT-COPYING".

EXCEPT AS OTHERWISE STATED IN A NEGOTIATED AGREEMENT: (A) THE SOFTWARE IS
PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT; AND (B) IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/ /**************************************************************************/

#include <linux/kernel.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>

#include "pvr_sync.h"
#include "mutex.h"
#include "lock.h"

/* We can't support this code when the MISR runs in atomic context because
 * PVRSyncFreeSync() may be called by sync_timeline_signal() which may be
 * scheduled by the MISR. PVRSyncFreeSync() needs to protect the handle
 * tables against modification and so uses the Linux bridge mutex.
 *
 * You can't lock a mutex in atomic context.
 */
#if !defined(PVR_LINUX_MISR_USING_WORKQUEUE) && \
    !defined(PVR_LINUX_MISR_USING_PRIVATE_WORKQUEUE)
#error The Android sync driver requires that the SGX MISR runs in wq context
#endif

/* The OS event object does not know which timeline a sync object is triggered
 * so we must keep a list to traverse all syncs for completion */
/* FIXME - Can this be avoided? */
static LIST_HEAD(gTimelineList);
static DEFINE_MUTEX(gTimelineListLock);

/* Keep track of any exports we made of the fences. To stop the handle
 * tables getting filled with dead fences, we need to remove the sync
 * handles from the per process data when the fence is released.
 */
static LIST_HEAD(gExportList);

struct ServicesExportCtx
{
	IMG_HANDLE			hKernelServices;
	IMG_HANDLE			hSyncInfo;
	struct sync_fence	*psFence;
	struct list_head	sExportList;
};

struct SyncInfoSnapshot
{
	IMG_UINT32 ui32ReadOpsPendingSnapshot;
	IMG_UINT32 ui32WriteOpsPendingSnapshot;
	IMG_UINT32 ui32ReadOps2PendingSnapshot;
};

static void PVRSyncSWTakeOp(PVRSRV_KERNEL_SYNC_INFO *psKernelSyncInfo)
{
	LinuxLockMutex(&gPVRSRVLock);
	psKernelSyncInfo->psSyncData->ui32WriteOpsPending++;
	LinuxUnLockMutex(&gPVRSRVLock);
}

static int PVRSyncSWFlushOp(PVRSRV_KERNEL_SYNC_INFO *psKernelSyncInfo, IMG_UINT32 ui32WOPSnapshot)
{
	/* FIXME: Waiting for long times in syscalls is normally a bad idea
	 *        Can this wait be punted up to userspace? (Return -EAGAIN)?
	 *        That doesn't work well with in-kernel users, or having to roll-back any
	 *        TakeOp calls that would rely on this flush? */
	LOOP_UNTIL_TIMEOUT(MAX_HW_TIME_US)
	{
		if (psKernelSyncInfo->psSyncData->ui32WriteOpsComplete >= ui32WOPSnapshot &&
		    psKernelSyncInfo->psSyncData->ui32ReadOpsComplete >= psKernelSyncInfo->psSyncData->ui32ReadOpsPending &&
			psKernelSyncInfo->psSyncData->ui32ReadOps2Pending >= psKernelSyncInfo->psSyncData->ui32ReadOps2Complete)
		{
			goto FlushComplete;
		}
		OSSleepms(1);
	} END_LOOP_UNTIL_TIMEOUT();
	PVR_DPF((PVR_DBG_ERROR, "%s: SW flush timed out", __func__));
	return 0;

FlushComplete:
	return 1;
}

static void PVRSyncSWCompleteOp(PVRSRV_KERNEL_SYNC_INFO *psKernelSyncInfo)
{
	psKernelSyncInfo->psSyncData->ui32WriteOpsComplete++;
}

static int
PVRSyncCompareSyncInfos(IMG_UINT32 ui32WOPA, IMG_UINT32 ui32WOPB)
{
	/* FIXME: The timestamp may not be a reliable method of determining
	 *        sync finish order between processes
	 */
	if (ui32WOPA == ui32WOPB)
		return 0;
	if (ui32WOPA < ui32WOPB)
		return 1;
	else
		return -1;
}

struct PVR_SYNC *PVRSyncCreateSync(struct PVR_SYNC_TIMELINE *obj,
                                  IMG_UINT32 ui32WOPSnapshot,
								  IMG_UINT32 ui32RequiredSWFenceValue)
{
	struct PVR_SYNC *psPt = (struct PVR_SYNC *)
		sync_pt_create(&obj->obj, sizeof(struct PVR_SYNC));

	psPt->ui32RequiredSWFenceValue = ui32RequiredSWFenceValue;
	psPt->ui32WOPSnapshot = ui32WOPSnapshot;

	/* Assume all syncs created with non-zero fence value are SW fences */
	if (ui32RequiredSWFenceValue)
	{
		unsigned long flags;
		psPt->eType = PVR_SYNC_TYPE_SW;
		spin_lock_irqsave(&obj->sActiveSWSyncsLock, flags);
		obj->ui32ActiveSWSyncs++;
		spin_unlock_irqrestore(&obj->sActiveSWSyncsLock, flags);
		if (ui32RequiredSWFenceValue > obj->ui32SWFenceValue+1)
		{
			PVR_DPF((PVR_DBG_ERROR, "%s: Creating SW sync point ahead in timeline (%u, current step %u)",
			                        __func__, ui32RequiredSWFenceValue, obj->ui32SWFenceValue));
		}
		PVRSyncSWTakeOp(obj->psSyncInfo);
		if (!PVRSyncSWFlushOp(obj->psSyncInfo, psPt->ui32WOPSnapshot))
		{
			PVR_DPF((PVR_DBG_ERROR, "%s: Failed to flush syncinfo", __func__));
		}
		psPt->bRequiresComplete = IMG_TRUE;
	}
	else
	{
		psPt->eType = PVR_SYNC_TYPE_HW;
		if (obj->ui32ActiveSWSyncs)
		{
			PVR_DPF((PVR_DBG_ERROR, "%s: Creating HW sync while SW sync still active", __func__));
		}
		psPt->bRequiresComplete = IMG_FALSE;
	}

	return psPt;
}

static void PVRSyncFreeSync(struct sync_pt *sync_pt)
{
	struct sync_fence *psFence = sync_pt->fence;
    struct list_head *psEntry, *psTmp;
	struct PVR_SYNC *psSync = (struct PVR_SYNC*)sync_pt;
	struct PVR_SYNC_TIMELINE *psTimeline = (struct PVR_SYNC_TIMELINE*)sync_pt->parent;

	LinuxLockMutex(&gPVRSRVLock);

    list_for_each_safe(psEntry, psTmp, &gExportList)
	{
		struct ServicesExportCtx *psExportCtx =
            container_of(psEntry, struct ServicesExportCtx, sExportList);
		PVRSRV_PER_PROCESS_DATA *psPerProc;
		PVRSRV_ERROR eError;

		/* Not this fence -- don't care */
		if(psExportCtx->psFence != psFence)
			continue;

		eError = PVRSRVLookupHandle(KERNEL_HANDLE_BASE,
									(IMG_PVOID *)&psPerProc,
									psExportCtx->hKernelServices,
									PVRSRV_HANDLE_TYPE_PERPROC_DATA);

		/* The handle's probably invalid because the context has already
	 	 * had resman run over it. In that case, the handle table should
		 * have been freed, and there's nothing for us to do.
		 */
		if(eError == PVRSRV_OK)
		{
			eError = PVRSRVReleaseHandle(psPerProc->psHandleBase,
										 psExportCtx->hSyncInfo,
										 PVRSRV_HANDLE_TYPE_SYNC_INFO);
			if(eError != PVRSRV_OK)
			{
				PVR_DPF((PVR_DBG_WARNING, "%s: PVRSRVReleaseHandle failed",
										  __func__));
			}
		}

		list_del(&psExportCtx->sExportList);
		kfree(psExportCtx);
	}
	/* FIXME: For sw syncs freed before completion, must call sw complete (woc++) */

	if (psSync->bRequiresComplete)
	{
		PVRSyncSWCompleteOp(psTimeline->psSyncInfo);
		psSync->bRequiresComplete = IMG_FALSE;
	}


	LinuxUnLockMutex(&gPVRSRVLock);
}

static struct sync_pt *PVRSyncDup(struct sync_pt *sync_pt)
{
	struct PVR_SYNC *psPt = (struct PVR_SYNC *)sync_pt;
	struct PVR_SYNC_TIMELINE *psObj =
		(struct PVR_SYNC_TIMELINE *)sync_pt->parent;

	return (struct sync_pt *)PVRSyncCreateSync(psObj, psPt->ui32WOPSnapshot, psPt->ui32RequiredSWFenceValue);
}

static int PVRSyncHasSignaled(struct sync_pt *sync_pt)
{
	struct PVR_SYNC *psPt = (struct PVR_SYNC *)sync_pt;
	struct PVR_SYNC_TIMELINE *psTimeline =
		(struct PVR_SYNC_TIMELINE*) sync_pt->parent;

	if (psTimeline->psSyncInfo->psSyncData->ui32WriteOpsComplete >= psPt->ui32WOPSnapshot &&
	        psTimeline->ui32SWFenceValue >= psPt->ui32RequiredSWFenceValue)
	{
		if(psPt->eType == PVR_SYNC_TYPE_SW)
		{
			unsigned long flags;
			if (psTimeline->ui32ActiveSWSyncs == 0)
			{
				PVR_DPF((PVR_DBG_ERROR, "%s: Invalid SW sync count", __func__));
				return 1;
			}
			spin_lock_irqsave(&psTimeline->sActiveSWSyncsLock, flags);
			psTimeline->ui32ActiveSWSyncs--;
			spin_unlock_irqrestore(&psTimeline->sActiveSWSyncsLock, flags);
			if (psPt->bRequiresComplete)
			{
				PVRSyncSWCompleteOp(psTimeline->psSyncInfo);
				psPt->bRequiresComplete = IMG_FALSE;
			}
			
		}
		return 1;
	}
	return 0;
}

static int PVRSyncCompare(struct sync_pt *a, struct sync_pt *b)
{
	return PVRSyncCompareSyncInfos(((struct PVR_SYNC*)a)->ui32WOPSnapshot,
								   ((struct PVR_SYNC*)b)->ui32WOPSnapshot);
}

static void PVRSyncPrintTimeline(struct seq_file *s, struct sync_timeline *sync_timeline)
{
	struct PVR_SYNC_TIMELINE* psTimeline = (struct PVR_SYNC_TIMELINE*)sync_timeline;
	seq_printf(s, "PVR_SYNC_TIMELINE \"%s\":\n", psTimeline->obj.name);
	seq_printf(s, "\tsync WOP:%u\n\t     WOC:%u\n",
	           psTimeline->psSyncInfo->psSyncData->ui32WriteOpsPending,
			   psTimeline->psSyncInfo->psSyncData->ui32WriteOpsComplete);
	seq_printf(s, "\tSW value: %u\n", psTimeline->ui32SWFenceValue);

}

static void PVRSyncPrint(struct seq_file *s, struct sync_pt *sync_pt)
{
	struct PVR_SYNC* psSync = (struct PVR_SYNC*)sync_pt;
	struct timeval tv = ktime_to_timeval(sync_pt->timestamp);
	seq_printf(s, "PVR_SYNC@%ld.%06ld\n", tv.tv_sec, tv.tv_usec);
	seq_printf(s, "\tWOP snapshot: %u\n", psSync->ui32WOPSnapshot);
	seq_printf(s, "\tSW fence value: %u\n", psSync->ui32RequiredSWFenceValue);
}

static int
PVRSyncFillDriverData(struct sync_pt *sync_pt, void *data, int size)
{
	struct PVR_SYNC *psPt = (struct PVR_SYNC *)sync_pt;
	/* FIXME: Is this used? */
	/* FIXME: Assumes only WOP/SW fence number needed, and they are in order
	 * & packed in the struct*/

	if (size < sizeof(psPt->ui32WOPSnapshot) + sizeof(psPt->ui32RequiredSWFenceValue))
		return -ENOMEM;

	memcpy(data, &psPt->ui32WOPSnapshot, sizeof(psPt->ui32WOPSnapshot) + sizeof(psPt->ui32RequiredSWFenceValue));

	return sizeof(psPt->ui32WOPSnapshot) + sizeof(psPt->ui32RequiredSWFenceValue);
	return 0;
}

struct sync_timeline_ops PVR_SYNC_TIMELINE_ops =
{
	.driver_name		= "pvr_sync",
	.dup				= PVRSyncDup,
	.has_signaled		= PVRSyncHasSignaled,
	.compare			= PVRSyncCompare,
	.print_obj			= PVRSyncPrintTimeline,
	.print_pt			= PVRSyncPrint,
	.fill_driver_data	= PVRSyncFillDriverData,
	.free_pt			= PVRSyncFreeSync,
};

static PVRSRV_ERROR PVRSyncInitServices(struct PVR_SYNC_TIMELINE* psTimeline)
{
	IMG_BOOL bCreated, bShared[PVRSRV_MAX_CLIENT_HEAPS];
	IMG_UINT32 ui32ClientHeapCount = 0;
	PVRSRV_DEVICE_NODE *psDeviceNode;
	PVRSRV_ERROR eError;
	PVRSRV_HEAP_INFO sHeapInfo[PVRSRV_MAX_CLIENT_HEAPS];
	LinuxLockMutex(&gPVRSRVLock);
	psTimeline->ui32Pid = OSGetCurrentProcessIDKM();

	eError = PVRSRVProcessConnect(psTimeline->ui32Pid, 0);

	if (eError != PVRSRV_OK)
	{
		PVR_DPF((PVR_DBG_ERROR, "%s: PVRSRVProcessConnect failed", __func__));
		goto out_unlock;
	}

	psTimeline->psPerProc = PVRSRVFindPerProcessData();

	if (psTimeline->psPerProc == NULL)
	{
		PVR_DPF((PVR_DBG_ERROR, "%s: PVRSRVFindPerProcessData failed", __func__));
		goto err_disconnect;
	}

	eError = PVRSRVAcquireDeviceDataKM(0, PVRSRV_DEVICE_TYPE_SGX, &psTimeline->hDevCookie);

	if (eError != PVRSRV_OK)
	{
		PVR_DPF((PVR_DBG_ERROR, "%s: PVRSRVAcquireDeviceDataKM failed", __func__));
		goto err_disconnect;
	}
	psDeviceNode = (PVRSRV_DEVICE_NODE*)psTimeline->hDevCookie;
	if (psDeviceNode == NULL)
	{
		PVR_DPF((PVR_DBG_ERROR, "%s: psDeviceNode == NULL", __func__));
		goto err_disconnect;
	}

	eError = PVRSRVCreateDeviceMemContextKM(psTimeline->hDevCookie,
	                                        psTimeline->psPerProc,
											&psTimeline->hDevMemContext,
											&ui32ClientHeapCount,
											&sHeapInfo[0],
											&bCreated,
											&bShared[0]);
	if (eError != PVRSRV_OK)
	{
		PVR_DPF((PVR_DBG_ERROR, "%s: PVRSRVCreateDeviceMemContextKM failed", __func__));
		goto err_disconnect;
	}

	if (psTimeline->hDevMemContext == NULL)
	{
		PVR_DPF((PVR_DBG_ERROR, "%s: hDevMemContext == NULL", __func__));
		goto err_disconnect;
	}

	LinuxUnLockMutex(&gPVRSRVLock);
	return PVRSRV_OK;

err_disconnect:
	PVRSRVProcessDisconnect(psTimeline->ui32Pid);
out_unlock:
	LinuxUnLockMutex(&gPVRSRVLock);
	return eError;
}

static void PVRSyncCloseServices(struct PVR_SYNC_TIMELINE *psTimeline)
{
	IMG_BOOL bDestroyed;
	LinuxLockMutex(&gPVRSRVLock);
	PVRSRVDestroyDeviceMemContextKM(psTimeline->hDevCookie, psTimeline->hDevMemContext, &bDestroyed);
	psTimeline->hDevMemContext = NULL;
	PVRSRVProcessDisconnect(psTimeline->ui32Pid);
	LinuxUnLockMutex(&gPVRSRVLock);
}

struct PVR_SYNC_TIMELINE *PVRSyncCreateTimeline(const IMG_CHAR *pszName)
{
	struct PVR_SYNC_TIMELINE* psTimeline;
	PVRSRV_ERROR eError;


	psTimeline = (struct PVR_SYNC_TIMELINE *)
	sync_timeline_create(&PVR_SYNC_TIMELINE_ops,
					 sizeof(struct PVR_SYNC_TIMELINE),
					 pszName);
	if (!psTimeline)
	{
		PVR_DPF((PVR_DBG_ERROR, "%s: sync_timeline_create failed", __func__));
		goto err_out;
	}

	eError = PVRSyncInitServices(psTimeline);

	if (eError != PVRSRV_OK)
	{
		PVR_DPF((PVR_DBG_ERROR, "%s: Failed to initialise PVR services", __func__));
		goto err_free_timeline;
	}
	
	/* FIXME: Do we need services mutex for alloc call? */
	eError = PVRSRVAllocSyncInfoKM(psTimeline->hDevCookie,
	                               psTimeline->hDevMemContext,
								   &psTimeline->psSyncInfo);
	
	if (eError != PVRSRV_OK)
	{
		PVR_DPF((PVR_DBG_ERROR, "%s: Failed to alloc sync info", __func__));
		goto err_close_services;
	}

	psTimeline->ui32SWFenceValue = 0;

	spin_lock_init(&psTimeline->sActiveSWSyncsLock);

	return psTimeline;

err_close_services:
	PVRSyncCloseServices(psTimeline);
err_free_timeline:
	/* FIXME: This leaks the timeline on error */
err_out:
	return NULL;
}

void PVRSyncDestroyTimeline(struct PVR_SYNC_TIMELINE *psTimeline)
{
	PVRSRVReleaseSyncInfoKM(psTimeline->psSyncInfo);
	PVRSRVDestroyDeviceMemContextKM(psTimeline->hDevCookie, psTimeline->hDevMemContext, NULL);
	PVRSRVProcessDisconnect(psTimeline->ui32Pid);
	sync_timeline_destroy(&psTimeline->obj);
}

static int PVRSyncOpen(struct inode *inode, struct file *file)
{
	struct PVR_SYNC_TIMELINE *psTimeline;
	IMG_CHAR task_comm[TASK_COMM_LEN+1];

	get_task_comm(task_comm, current);


	psTimeline = PVRSyncCreateTimeline(task_comm);
	if (!psTimeline)
		return -ENOMEM;

	mutex_lock(&gTimelineListLock);
	list_add_tail(&psTimeline->sTimelineList, &gTimelineList);
	mutex_unlock(&gTimelineListLock);

	file->private_data = psTimeline;
	return 0;
}

static int PVRSyncRelease(struct inode *inode, struct file *file)
{
	struct PVR_SYNC_TIMELINE *psTimeline = file->private_data;



	mutex_lock(&gTimelineListLock);
	list_del(&psTimeline->sTimelineList);
	mutex_unlock(&gTimelineListLock);
	
	
	PVRSyncDestroyTimeline(psTimeline);
	return 0;
}

static long
PVRSyncIOCTLCreate(struct PVR_SYNC_TIMELINE *psObj, unsigned long ulArg)
{
	struct PVR_SYNC_CREATE_IOCTL_DATA sData;
	int err = 0, iFd = get_unused_fd();
	struct sync_fence *psFence;
	struct sync_pt *psPt;

	if (iFd < 0)
	{
		PVR_DPF((PVR_DBG_ERROR, "%s: Failed to find unused fd (%d)",
								__func__, iFd));
		err = -EFAULT;
		goto err_out;
	}

	/* FIXME: Add AccessOK checks */

	if (copy_from_user(&sData, (void __user *)ulArg, sizeof(sData)))
	{
		err = -EFAULT;
		goto err_out;
	}

	psPt = (struct sync_pt*)PVRSyncCreateSync(psObj, psObj->psSyncInfo->psSyncData->ui32WriteOpsPending,
	                         sData.value);
	if (!psPt) {
		err = -ENOMEM;
		goto err_out;
	}

	sData.name[sizeof(sData.name) - 1] = '\0';
	psFence = sync_fence_create(sData.name, psPt);
	if (!psFence)
	{
		sync_pt_free(psPt);
		err = -ENOMEM;
		goto err_out;
	}

	sData.fence = iFd;
	if (copy_to_user((void __user *)ulArg, &sData, sizeof(sData)))
	{
		sync_fence_put(psFence);
		err = -EFAULT;
		goto err_out;
	}

	sync_fence_install(psFence, iFd);

err_out:
	return err;
}

static long
PVRSyncIOCTLImportTimelineSync(struct PVR_SYNC_TIMELINE *psObj, unsigned long ulArg)
{
	PVRSRV_KERNEL_SYNC_INFO *psKernelSyncInfo = NULL;
	IMG_UINT32 ui32PID = OSGetCurrentProcessIDKM();
	struct PVR_SYNC_DRV_IMPORT_IOCTL_DATA sData;
	struct ServicesExportCtx *psExportCtx;
	PVRSRV_PER_PROCESS_DATA *psPerProc;
	IMG_HANDLE hSyncInfo;
	PVRSRV_ERROR eError;
	int err = -EFAULT;

	/* FIXME: Add AccessOK checks */

	if (copy_from_user(&sData, (void __user *)ulArg, sizeof(sData)))
		goto err_out;

	/* FIXME: Do we want to use the snapshot WOP value? */
	psKernelSyncInfo = psObj->psSyncInfo;
	PVR_ASSERT(psKernelSyncInfo != NULL);

	LinuxLockMutex(&gPVRSRVLock);

	eError = PVRSRVLookupHandle(KERNEL_HANDLE_BASE,
								(IMG_PVOID *)&psPerProc,
								sData.hKernelServices,
								PVRSRV_HANDLE_TYPE_PERPROC_DATA);
	if(eError != PVRSRV_OK)
	{
		PVR_DPF((PVR_DBG_ERROR, "%s: Invalid kernel services handle (%d)",
								__func__, eError));
		goto err_unlock;
	}

	if(psPerProc->ui32PID != ui32PID)
	{
		PVR_DPF((PVR_DBG_ERROR, "%s: Process %d tried to access data "
								"belonging to process %d", __func__,
								ui32PID, psPerProc->ui32PID));
		goto err_unlock;
	}

	/* Use MULTI in case userspace keeps exporting the same fence. */
	eError = PVRSRVAllocHandle(psPerProc->psHandleBase,
							   &hSyncInfo,
							   psKernelSyncInfo,
							   PVRSRV_HANDLE_TYPE_SYNC_INFO,
							   PVRSRV_HANDLE_ALLOC_FLAG_MULTI);
	if (eError != PVRSRV_OK)
	{
		PVR_DPF((PVR_DBG_ERROR, "%s: Failed to alloc handle for SYNC_INFO",
								__func__));
		goto err_unlock;
	}

	sData.hSyncInfo = hSyncInfo;
	if (copy_to_user((void __user *)ulArg, &sData, sizeof(sData)))
		goto err_release_handle;

	psExportCtx = kmalloc(sizeof(*psExportCtx), GFP_KERNEL);
	if(!psExportCtx)
	{
		PVR_DPF((PVR_DBG_ERROR, "%s: Failed to allocate psExportCtx",
								__func__));
		err = -ENOMEM;
		goto err_release_handle;
	}

	psExportCtx->hKernelServices = sData.hKernelServices;
	psExportCtx->hSyncInfo = hSyncInfo;

	list_add_tail(&psExportCtx->sExportList, &gExportList);

	err = 0;
err_unlock:
	LinuxUnLockMutex(&gPVRSRVLock);
err_out:
	return err;

err_release_handle:
	PVRSRVReleaseHandle(psPerProc->psHandleBase,
						hSyncInfo,
						PVRSRV_HANDLE_TYPE_SYNC_INFO);
	goto err_unlock;
}

static long
PVRSyncIOCTLImport(struct PVR_SYNC_TIMELINE *psObj, unsigned long ulArg)
{
	PVRSRV_KERNEL_SYNC_INFO *psKernelSyncInfo = NULL;
	IMG_UINT32 ui32PID = OSGetCurrentProcessIDKM();
	struct PVR_SYNC_IMPORT_IOCTL_DATA sData;
	struct ServicesExportCtx *psExportCtx;
	PVRSRV_PER_PROCESS_DATA *psPerProc;
	IMG_HANDLE hSyncInfo;
	PVRSRV_ERROR eError;
	int err = -EFAULT;

	/* FIXME: Add AccessOK checks */

	if (copy_from_user(&sData, (void __user *)ulArg, sizeof(sData)))
		goto err_out;

	/* FIXME: Do we want to use the snapshot WOP value? */
	psKernelSyncInfo = psObj->psSyncInfo;
	PVR_ASSERT(psKernelSyncInfo != NULL);

	LinuxLockMutex(&gPVRSRVLock);

	eError = PVRSRVLookupHandle(KERNEL_HANDLE_BASE,
								(IMG_PVOID *)&psPerProc,
								sData.hKernelServices,
								PVRSRV_HANDLE_TYPE_PERPROC_DATA);
	if(eError != PVRSRV_OK)
	{
		PVR_DPF((PVR_DBG_ERROR, "%s: Invalid kernel services handle (%d)",
								__func__, eError));
		goto err_unlock;
	}

	if(psPerProc->ui32PID != ui32PID)
	{
		PVR_DPF((PVR_DBG_ERROR, "%s: Process %d tried to access data "
								"belonging to process %d", __func__,
								ui32PID, psPerProc->ui32PID));
		goto err_unlock;
	}

	/* Use MULTI in case userspace keeps exporting the same fence. */
	eError = PVRSRVAllocHandle(psPerProc->psHandleBase,
							   &hSyncInfo,
							   psKernelSyncInfo,
							   PVRSRV_HANDLE_TYPE_SYNC_INFO,
							   PVRSRV_HANDLE_ALLOC_FLAG_MULTI);
	if (eError != PVRSRV_OK)
	{
		PVR_DPF((PVR_DBG_ERROR, "%s: Failed to alloc handle for SYNC_INFO",
								__func__));
		goto err_unlock;
	}

	sData.hSyncInfo = hSyncInfo;
	if (copy_to_user((void __user *)ulArg, &sData, sizeof(sData)))
		goto err_release_handle;

	psExportCtx = kmalloc(sizeof(*psExportCtx), GFP_KERNEL);
	if(!psExportCtx)
	{
		PVR_DPF((PVR_DBG_ERROR, "%s: Failed to allocate psExportCtx",
								__func__));
		err = -ENOMEM;
		goto err_release_handle;
	}

	psExportCtx->hKernelServices = sData.hKernelServices;
	psExportCtx->hSyncInfo = hSyncInfo;

	list_add_tail(&psExportCtx->sExportList, &gExportList);

	err = 0;
err_unlock:
	LinuxUnLockMutex(&gPVRSRVLock);
err_out:
	return err;

err_release_handle:
	PVRSRVReleaseHandle(psPerProc->psHandleBase,
						hSyncInfo,
						PVRSRV_HANDLE_TYPE_SYNC_INFO);
	goto err_unlock;
}

static long
PVRSyncIOCTLDebug(struct PVR_SYNC_TIMELINE *psObj, unsigned long ulArg)
{
	PVRSRV_KERNEL_SYNC_INFO *psKernelSyncInfo = NULL;
	struct PVR_SYNC_DEBUG_IOCTL_DATA sData;
	int err = -EFAULT;

	/* FIXME: Add AccessOK checks */

	if(copy_from_user(&sData, (void __user *)ulArg, sizeof(sData)))
		goto err_out;

	/* FIXME: Do we want to use the snapshot WOP value? */
	psKernelSyncInfo = psObj->psSyncInfo;
	PVR_ASSERT(psKernelSyncInfo != NULL);

	/* The sync refcount is valid as long as the FenceFD stays open,
	 * so we can access it directly without worrying about it being
	 * freed.
	 */
	sData.sSyncData					= *psKernelSyncInfo->psSyncData;
	sData.sReadOpsCompleteDevVAddr	= psKernelSyncInfo->sReadOpsCompleteDevVAddr;
	sData.sReadOps2CompleteDevVAddr	= psKernelSyncInfo->sReadOps2CompleteDevVAddr;
	sData.sWriteOpsCompleteDevVAddr	= psKernelSyncInfo->sWriteOpsCompleteDevVAddr;

	if(copy_to_user((void __user *)ulArg, &sData, sizeof(sData)))
		goto err_out;

	err = 0;
err_out:
	return err;
}
static void
PVRSyncTimelineInc(struct PVR_SYNC_TIMELINE *psObj, u32 value)
{
	if (value != 1)
	{
		PVR_DPF((PVR_DBG_ERROR, "%s: increment called with non-1 arg (%u)", __func__, value));
	}
	psObj->ui32SWFenceValue++;
	sync_timeline_signal(&psObj->obj);
}

static long
PVRSyncIOCTLInc(struct PVR_SYNC_TIMELINE *psObj, unsigned long ulArg)
{

	u32 value;
	if (copy_from_user(&value, (void __user *)ulArg, sizeof(u32)))
		return -EFAULT;
	PVRSyncTimelineInc(psObj, value);
	return 0;
}



static long
PVRSyncIOCTL(struct file *file, unsigned int cmd, unsigned long arg)
{
	struct PVR_SYNC_TIMELINE *psTimeline = file->private_data;

	switch (cmd)
	{
		case PVR_SYNC_IOC_CREATE_FENCE:
			return PVRSyncIOCTLCreate(psTimeline, arg);
		case PVR_SYNC_IOC_INC:
			return PVRSyncIOCTLInc(psTimeline, arg);
		case PVR_SYNC_IOC_IMPORT_FENCE:
			return PVRSyncIOCTLImport(psTimeline, arg);
		case PVR_SYNC_IOC_IMPORT_SYNCINFO:
			return PVRSyncIOCTLImportTimelineSync(psTimeline, arg);
		case PVR_SYNC_IOC_DEBUG_FENCE:
			return PVRSyncIOCTLDebug(psTimeline, arg);
		default:
			return -ENOTTY;
	}
}

static const struct file_operations sPVRSyncFOps =
{
	.owner			= THIS_MODULE,
	.open			= PVRSyncOpen,
	.release		= PVRSyncRelease,
	.unlocked_ioctl	= PVRSyncIOCTL,
};

static struct miscdevice sPVRSyncDev =
{
	.minor	= MISC_DYNAMIC_MINOR,
	.name	= "pvr_sync",
	.fops	= &sPVRSyncFOps,
};

int PVRSyncDeviceInit(void)
{
	return misc_register(&sPVRSyncDev);
}

void PVRSyncDeviceDeInit(void)
{
	misc_deregister(&sPVRSyncDev);
}

void PVRSyncUpdateAllSyncs(void)
{
	struct list_head *psEntry;

	mutex_lock(&gTimelineListLock);

	list_for_each(psEntry, &gTimelineList)
	{
		struct PVR_SYNC_TIMELINE *psTimeline =
			container_of(psEntry, struct PVR_SYNC_TIMELINE, sTimelineList);
		sync_timeline_signal((struct sync_timeline *)psTimeline);
	}

	mutex_unlock(&gTimelineListLock);
}

/* sw_sync compatability */

struct sync_timeline*
sw_sync_create_timeline(const char* name)
{
	return (struct sync_timeline*)PVRSyncCreateTimeline(name);
}

void sw_sync_timeline_inc(struct PVR_SYNC_TIMELINE *obj, u32 value)
{
	PVRSyncTimelineInc(obj, value);
}

struct sync_pt*
sw_sync_pt_create(struct PVR_SYNC_TIMELINE *obj, u32 value)
{
	return (struct sync_pt*)PVRSyncCreateSync(obj, obj->psSyncInfo->psSyncData->ui32WriteOpsPending, value);
}
