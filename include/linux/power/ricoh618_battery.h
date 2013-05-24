/*
 * include/linux/power/ricoh618-battery.c
 *
 * RICOH RN5T618 Charger Driver
 *
 * Copyright (C) 2012-2013 RICOH COMPANY,LTD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#ifndef __LINUX_POWER_RICOH618_H_
#define __LINUX_POWER_RICOH618_H_

//#include <linux/power_supply.h>
//#include <linux/types.h>

//Defined buttery information
#define	ADC_VDD_MV	2800
#define	MIN_VOLTAGE	3100
#define	MAX_VOLTAGE	4200

//618 Register information
//bank 0
//for ADC
#define	INTEN_REG			0x9D
#define	EN_ADCIR3_REG	0x8A
#define	ADCCNT3_REG		0x66
#define	VBATDATAH_REG	0x6A
#define	VBATDATAL_REG	0x6B

#define CHGSTATE_REG		0xBD

#define	FG_CTRL_REG		0xE0
#define	SOC_REG			0xE1
#define	TT_EMPTY_H_REG	0xE7
#define	TT_EMPTY_L_REG	0xE8
#define	TT_FULL_H_REG		0xE9
#define	TT_FULL_L_REG		0xEA
#define	VOLTAGE_1_REG	0xEB
#define	VOLTAGE_2_REG	0xEC
#define	TEMP_1_REG		0xED
#define	TEMP_2_REG		0xEE

//bank 1
//Top address for battery initial setting
#define	BAT_INIT_TOP_REG	0xBC
////////////////////////////

//detailed status in CHGSTATE (0xBD)
enum ChargeState {
	CHG_STATE_CHG_OFF = 0,
	CHG_STATE_CHG_READY_VADP,
	CHG_STATE_CHG_TRICKLE,
	CHG_STATE_CHG_RAPID,
	CHG_STATE_CHG_COMPLETE,
	CHG_STATE_SUSPEND,
	CHG_STATE_VCHG_OVER_VOL,
	CHG_STATE_BAT_ERROR,
	CHG_STATE_NO_BAT,
	CHG_STATE_BAT_OVER_VOL,
	CHG_STATE_BAT_TEMP_ERR,
	CHG_STATE_DIE_ERR,
	CHG_STATE_DIE_SHUTDOWN,
	CHG_STATE_NO_BAT2,
	CHG_STATE_CHG_READY_VUSB,
};

enum SuppyState {
	SUPPLY_STATE_BAT = 0,
	SUPPLY_STATE_ADP,
	SUPPLY_STATE_USB,
} ;

struct ricoh618_battery_platform_data {
	int         alarm_vol_mv;
	int         multiple;
	unsigned long       monitor_time;
};



#endif
