/*
 * pdma.c
 */
#include <common.h>
#include <pdma.h>
#include <asm/jzsoc.h>

void nemc_channel_cfg(unsigned char *source_addr, unsigned char *dest_addr,
			unsigned int trans_count, int mode)
{
	REG_PDMAC_DTC(PDMA_NEMC_CHANNEL) = trans_count;
	REG_PDMAC_DRT(PDMA_NEMC_CHANNEL) = PDMAC_DRT_RT_AUTO;

	if (mode & NEMC_TO_TCSM) {
		REG_PDMAC_DSA(PDMA_NEMC_CHANNEL) = CPHYSADDR((u32)source_addr);
		REG_PDMAC_DTA(PDMA_NEMC_CHANNEL) = TPHYSADDR((u32)dest_addr);

		REG_PDMAC_DCMD(PDMA_NEMC_CHANNEL) &= ~(PDMAC_DCMD_SID_MASK | PDMAC_DCMD_DID_MASK |
						PDMAC_DCMD_SAI | PDMAC_DCMD_DAI);
		REG_PDMAC_DCMD(PDMA_NEMC_CHANNEL) |= PDMAC_DCMD_SID_SPECIAL | PDMAC_DCMD_DID_TCSM |
						PDMAC_DCMD_DAI;
	} else { /* TCSM_TO_NEMC */
		REG_PDMAC_DSA(PDMA_NEMC_CHANNEL) = TPHYSADDR((u32)source_addr);
		REG_PDMAC_DTA(PDMA_NEMC_CHANNEL) = CPHYSADDR((u32)dest_addr);

		REG_PDMAC_DCMD(PDMA_NEMC_CHANNEL) &= ~(PDMAC_DCMD_SID_MASK | PDMAC_DCMD_DID_MASK |
						PDMAC_DCMD_SAI | PDMAC_DCMD_DAI);
		REG_PDMAC_DCMD(PDMA_NEMC_CHANNEL) |= PDMAC_DCMD_SID_TCSM | PDMAC_DCMD_DID_SPECIAL |
						PDMAC_DCMD_SAI;
	}
}

void bch_channel_cfg(unsigned char *source_addr, unsigned char *dest_addr,
			unsigned int trans_count, int mode)
{
	REG_PDMAC_DTC(PDMA_BCH_CHANNEL) = trans_count;
	REG_PDMAC_DRT(PDMA_BCH_CHANNEL) = PDMAC_DRT_RT_AUTO;

	if (mode & BCH_TO_TCSM) {
		REG_PDMAC_DSA(PDMA_BCH_CHANNEL) = CPHYSADDR((u32)source_addr);
		REG_PDMAC_DTA(PDMA_BCH_CHANNEL) = TPHYSADDR((u32)dest_addr);

		REG_PDMAC_DCMD(PDMA_BCH_CHANNEL) &= ~(PDMAC_DCMD_SID_MASK | PDMAC_DCMD_DID_MASK |
						PDMAC_DCMD_SAI | PDMAC_DCMD_DAI);
		REG_PDMAC_DCMD(PDMA_BCH_CHANNEL) |= PDMAC_DCMD_SID_SPECIAL | PDMAC_DCMD_DID_TCSM |
						PDMAC_DCMD_SAI | PDMAC_DCMD_DAI;
	} else { /* TCSM_TO_BCH */
		REG_PDMAC_DSA(PDMA_BCH_CHANNEL) = TPHYSADDR((u32)source_addr);
		REG_PDMAC_DTA(PDMA_BCH_CHANNEL) = CPHYSADDR((u32)dest_addr);

		REG_PDMAC_DCMD(PDMA_BCH_CHANNEL) &= ~(PDMAC_DCMD_SID_MASK | PDMAC_DCMD_DID_MASK |
						PDMAC_DCMD_SAI | PDMAC_DCMD_DAI);
		REG_PDMAC_DCMD(PDMA_BCH_CHANNEL) |= PDMAC_DCMD_SID_TCSM | PDMAC_DCMD_DID_SPECIAL |
						PDMAC_DCMD_SAI;
	}
}

void ddr_channel_cfg(unsigned char *source_addr, unsigned char *dest_addr,
			unsigned int trans_count, int mode)
{
	REG_PDMAC_DTC(PDMA_DDR_CHANNEL) = trans_count;
	REG_PDMAC_DRT(PDMA_DDR_CHANNEL) = PDMAC_DRT_RT_AUTO;

	if (mode & DDR_TO_TCSM) {
		REG_PDMAC_DSA(PDMA_DDR_CHANNEL) = CPHYSADDR((u32)source_addr);
		REG_PDMAC_DTA(PDMA_DDR_CHANNEL) = TPHYSADDR((u32)dest_addr);
	} else { /* TCSM_TO_DDR */
		REG_PDMAC_DSA(PDMA_DDR_CHANNEL) = TPHYSADDR((u32)source_addr);
		REG_PDMAC_DTA(PDMA_DDR_CHANNEL) = CPHYSADDR((u32)dest_addr);
	}
}

void pdma_channel_init(void)
{
	/* Enable Special Channel 0 and Channel 1 */
	REG_PDMAC_DMAC |= PDMAC_DMAC_CH01;

	REG_PDMAC_DCCS(PDMA_DDR_CHANNEL) |= PDMAC_DCCS_NDES;

	__pdmac_channel_programmable_set(PDMA_NEMC_CHANNEL);
	__pdmac_channel_programmable_set(PDMA_BCH_CHANNEL);
	__pdmac_channel_programmable_set(PDMA_DDR_CHANNEL);
			
	REG_PDMAC_DCMD(PDMA_NEMC_CHANNEL) = PDMAC_DCMD_SWDH_32 | PDMAC_DCMD_DWDH_32 |
					PDMAC_DCMD_TSZ_AUTO | PDMAC_DCMD_TIE;
	REG_PDMAC_DCMD(PDMA_BCH_CHANNEL) = PDMAC_DCMD_SWDH_32 | PDMAC_DCMD_DWDH_32 |
					PDMAC_DCMD_TSZ_AUTO | PDMAC_DCMD_TIE;
	REG_PDMAC_DCMD(PDMA_DDR_CHANNEL) = PDMAC_DCMD_SAI | PDMAC_DCMD_DAI |
					PDMAC_DCMD_SWDH_32 | PDMAC_DCMD_DWDH_32 |
					PDMAC_DCMD_TSZ_AUTO | PDMAC_DCMD_TIE;
}