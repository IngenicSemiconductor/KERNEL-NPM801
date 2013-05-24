/*
 * Copyright (C) 2009-2010 Amit Kucheria <amit.kucheria@canonical.com>
 * Copyright (C) 2010 Freescale Semiconductor, Inc.
 *
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */

#ifndef __MACH_IOMUX_MX23_H__
#define __MACH_IOMUX_MX23_H__

#include <mach/iomux.h>

/*
 * The naming convention for the pad modes is MX23_PAD_<padname>__<padmode>
 * If <padname> or <padmode> refers to a GPIO, it is named GPIO_<unit>_<num>
 * See also iomux.h
 *
 *									BANK	PIN	MUX
 */
/* MUXSEL_0 */
#define MX23_PAD_GPMI_D00__GPMI_D00		MXS_IOMUX_PAD_NAKED(0,  0, PAD_MUXSEL_0)
#define MX23_PAD_GPMI_D01__GPMI_D01		MXS_IOMUX_PAD_NAKED(0,  1, PAD_MUXSEL_0)
#define MX23_PAD_GPMI_D02__GPMI_D02		MXS_IOMUX_PAD_NAKED(0,  2, PAD_MUXSEL_0)
#define MX23_PAD_GPMI_D03__GPMI_D03		MXS_IOMUX_PAD_NAKED(0,  3, PAD_MUXSEL_0)
#define MX23_PAD_GPMI_D04__GPMI_D04		MXS_IOMUX_PAD_NAKED(0,  4, PAD_MUXSEL_0)
#define MX23_PAD_GPMI_D05__GPMI_D05		MXS_IOMUX_PAD_NAKED(0,  5, PAD_MUXSEL_0)
#define MX23_PAD_GPMI_D06__GPMI_D06		MXS_IOMUX_PAD_NAKED(0,  6, PAD_MUXSEL_0)
#define MX23_PAD_GPMI_D07__GPMI_D07		MXS_IOMUX_PAD_NAKED(0,  7, PAD_MUXSEL_0)
#define MX23_PAD_GPMI_D08__GPMI_D08		MXS_IOMUX_PAD_NAKED(0,  8, PAD_MUXSEL_0)
#define MX23_PAD_GPMI_D09__GPMI_D09		MXS_IOMUX_PAD_NAKED(0,  9, PAD_MUXSEL_0)
#define MX23_PAD_GPMI_D10__GPMI_D10		MXS_IOMUX_PAD_NAKED(0, 10, PAD_MUXSEL_0)
#define MX23_PAD_GPMI_D11__GPMI_D11		MXS_IOMUX_PAD_NAKED(0, 11, PAD_MUXSEL_0)
#define MX23_PAD_GPMI_D12__GPMI_D12		MXS_IOMUX_PAD_NAKED(0, 12, PAD_MUXSEL_0)
#define MX23_PAD_GPMI_D13__GPMI_D13		MXS_IOMUX_PAD_NAKED(0, 13, PAD_MUXSEL_0)
#define MX23_PAD_GPMI_D14__GPMI_D14		MXS_IOMUX_PAD_NAKED(0, 14, PAD_MUXSEL_0)
#define MX23_PAD_GPMI_D15__GPMI_D15		MXS_IOMUX_PAD_NAKED(0, 15, PAD_MUXSEL_0)
#define MX23_PAD_GPMI_CLE__GPMI_CLE		MXS_IOMUX_PAD_NAKED(0, 16, PAD_MUXSEL_0)
#define MX23_PAD_GPMI_ALE__GPMI_ALE		MXS_IOMUX_PAD_NAKED(0, 17, PAD_MUXSEL_0)
#define MX23_PAD_GPMI_CE2N__GPMI_CE2N		MXS_IOMUX_PAD_NAKED(0, 18, PAD_MUXSEL_0)
#define MX23_PAD_GPMI_RDY0__GPMI_RDY0		MXS_IOMUX_PAD_NAKED(0, 19, PAD_MUXSEL_0)
#define MX23_PAD_GPMI_RDY1__GPMI_RDY1		MXS_IOMUX_PAD_NAKED(0, 20, PAD_MUXSEL_0)
#define MX23_PAD_GPMI_RDY2__GPMI_RDY2		MXS_IOMUX_PAD_NAKED(0, 21, PAD_MUXSEL_0)
#define MX23_PAD_GPMI_RDY3__GPMI_RDY3		MXS_IOMUX_PAD_NAKED(0, 22, PAD_MUXSEL_0)
#define MX23_PAD_GPMI_WPN__GPMI_WPN		MXS_IOMUX_PAD_NAKED(0, 23, PAD_MUXSEL_0)
#define MX23_PAD_GPMI_WRN__GPMI_WRN		MXS_IOMUX_PAD_NAKED(0, 24, PAD_MUXSEL_0)
#define MX23_PAD_GPMI_RDN__GPMI_RDN		MXS_IOMUX_PAD_NAKED(0, 25, PAD_MUXSEL_0)
#define MX23_PAD_AUART1_CTS__AUART1_CTS		MXS_IOMUX_PAD_NAKED(0, 26, PAD_MUXSEL_0)
#define MX23_PAD_AUART1_RTS__AUART1_RTS		MXS_IOMUX_PAD_NAKED(0, 27, PAD_MUXSEL_0)
#define MX23_PAD_AUART1_RX__AUART1_RX		MXS_IOMUX_PAD_NAKED(0, 28, PAD_MUXSEL_0)
#define MX23_PAD_AUART1_TX__AUART1_TX		MXS_IOMUX_PAD_NAKED(0, 29, PAD_MUXSEL_0)
#define MX23_PAD_I2C_SCL__I2C_SCL		MXS_IOMUX_PAD_NAKED(0, 30, PAD_MUXSEL_0)
#define MX23_PAD_I2C_SDA__I2C_SDA		MXS_IOMUX_PAD_NAKED(0, 31, PAD_MUXSEL_0)

#define MX23_PAD_LCD_D00__LCD_D00		MXS_IOMUX_PAD_NAKED(1,  0, PAD_MUXSEL_0)
#define MX23_PAD_LCD_D01__LCD_D01		MXS_IOMUX_PAD_NAKED(1,  1, PAD_MUXSEL_0)
#define MX23_PAD_LCD_D02__LCD_D02		MXS_IOMUX_PAD_NAKED(1,  2, PAD_MUXSEL_0)
#define MX23_PAD_LCD_D03__LCD_D03		MXS_IOMUX_PAD_NAKED(1,  3, PAD_MUXSEL_0)
#define MX23_PAD_LCD_D04__LCD_D04		MXS_IOMUX_PAD_NAKED(1,  4, PAD_MUXSEL_0)
#define MX23_PAD_LCD_D05__LCD_D05		MXS_IOMUX_PAD_NAKED(1,  5, PAD_MUXSEL_0)
#define MX23_PAD_LCD_D06__LCD_D06		MXS_IOMUX_PAD_NAKED(1,  6, PAD_MUXSEL_0)
#define MX23_PAD_LCD_D07__LCD_D07		MXS_IOMUX_PAD_NAKED(1,  7, PAD_MUXSEL_0)
#define MX23_PAD_LCD_D08__LCD_D08		MXS_IOMUX_PAD_NAKED(1,  8, PAD_MUXSEL_0)
#define MX23_PAD_LCD_D09__LCD_D09		MXS_IOMUX_PAD_NAKED(1,  9, PAD_MUXSEL_0)
#define MX23_PAD_LCD_D10__LCD_D10		MXS_IOMUX_PAD_NAKED(1, 10, PAD_MUXSEL_0)
#define MX23_PAD_LCD_D11__LCD_D11		MXS_IOMUX_PAD_NAKED(1, 11, PAD_MUXSEL_0)
#define MX23_PAD_LCD_D12__LCD_D12		MXS_IOMUX_PAD_NAKED(1, 12, PAD_MUXSEL_0)
#define MX23_PAD_LCD_D13__LCD_D13		MXS_IOMUX_PAD_NAKED(1, 13, PAD_MUXSEL_0)
#define MX23_PAD_LCD_D14__LCD_D14		MXS_IOMUX_PAD_NAKED(1, 14, PAD_MUXSEL_0)
#define MX23_PAD_LCD_D15__LCD_D15		MXS_IOMUX_PAD_NAKED(1, 15, PAD_MUXSEL_0)
#define MX23_PAD_LCD_D16__LCD_D16		MXS_IOMUX_PAD_NAKED(1, 16, PAD_MUXSEL_0)
#define MX23_PAD_LCD_D17__LCD_D17		MXS_IOMUX_PAD_NAKED(1, 17, PAD_MUXSEL_0)
#define MX23_PAD_LCD_RESET__LCD_RESET		MXS_IOMUX_PAD_NAKED(1, 18, PAD_MUXSEL_0)
#define MX23_PAD_LCD_RS__LCD_RS			MXS_IOMUX_PAD_NAKED(1, 19, PAD_MUXSEL_0)
#define MX23_PAD_LCD_WR__LCD_WR			MXS_IOMUX_PAD_NAKED(1, 20, PAD_MUXSEL_0)
#define MX23_PAD_LCD_CS__LCD_CS			MXS_IOMUX_PAD_NAKED(1, 21, PAD_MUXSEL_0)
#define MX23_PAD_LCD_DOTCK__LCD_DOTCK		MXS_IOMUX_PAD_NAKED(1, 22, PAD_MUXSEL_0)
#define MX23_PAD_LCD_ENABLE__LCD_ENABLE		MXS_IOMUX_PAD_NAKED(1, 23, PAD_MUXSEL_0)
#define MX23_PAD_LCD_HSYNC__LCD_HSYNC		MXS_IOMUX_PAD_NAKED(1, 24, PAD_MUXSEL_0)
#define MX23_PAD_LCD_VSYNC__LCD_VSYNC		MXS_IOMUX_PAD_NAKED(1, 25, PAD_MUXSEL_0)
#define MX23_PAD_PWM0__PWM0			MXS_IOMUX_PAD_NAKED(1, 26, PAD_MUXSEL_0)
#define MX23_PAD_PWM1__PWM1			MXS_IOMUX_PAD_NAKED(1, 27, PAD_MUXSEL_0)
#define MX23_PAD_PWM2__PWM2			MXS_IOMUX_PAD_NAKED(1, 28, PAD_MUXSEL_0)
#define MX23_PAD_PWM3__PWM3			MXS_IOMUX_PAD_NAKED(1, 29, PAD_MUXSEL_0)
#define MX23_PAD_PWM4__PWM4			MXS_IOMUX_PAD_NAKED(1, 30, PAD_MUXSEL_0)

#define MX23_PAD_SSP1_CMD__SSP1_CMD		MXS_IOMUX_PAD_NAKED(2,  0, PAD_MUXSEL_0)
#define MX23_PAD_SSP1_DETECT__SSP1_DETECT	MXS_IOMUX_PAD_NAKED(2,  1, PAD_MUXSEL_0)
#define MX23_PAD_SSP1_DATA0__SSP1_DATA0		MXS_IOMUX_PAD_NAKED(2,  2, PAD_MUXSEL_0)
#define MX23_PAD_SSP1_DATA1__SSP1_DATA1		MXS_IOMUX_PAD_NAKED(2,  3, PAD_MUXSEL_0)
#define MX23_PAD_SSP1_DATA2__SSP1_DATA2		MXS_IOMUX_PAD_NAKED(2,  4, PAD_MUXSEL_0)
#define MX23_PAD_SSP1_DATA3__SSP1_DATA3		MXS_IOMUX_PAD_NAKED(2,  5, PAD_MUXSEL_0)
#define MX23_PAD_SSP1_SCK__SSP1_SCK		MXS_IOMUX_PAD_NAKED(2,  6, PAD_MUXSEL_0)
#define MX23_PAD_ROTARYA__ROTARYA		MXS_IOMUX_PAD_NAKED(2,  7, PAD_MUXSEL_0)
#define MX23_PAD_ROTARYB__ROTARYB		MXS_IOMUX_PAD_NAKED(2,  8, PAD_MUXSEL_0)
#define MX23_PAD_EMI_A00__EMI_A00		MXS_IOMUX_PAD_NAKED(2,  9, PAD_MUXSEL_0)
#define MX23_PAD_EMI_A01__EMI_A01		MXS_IOMUX_PAD_NAKED(2, 10, PAD_MUXSEL_0)
#define MX23_PAD_EMI_A02__EMI_A02		MXS_IOMUX_PAD_NAKED(2, 11, PAD_MUXSEL_0)
#define MX23_PAD_EMI_A03__EMI_A03		MXS_IOMUX_PAD_NAKED(2, 12, PAD_MUXSEL_0)
#define MX23_PAD_EMI_A04__EMI_A04		MXS_IOMUX_PAD_NAKED(2, 13, PAD_MUXSEL_0)
#define MX23_PAD_EMI_A05__EMI_A05		MXS_IOMUX_PAD_NAKED(2, 14, PAD_MUXSEL_0)
#define MX23_PAD_EMI_A06__EMI_A06		MXS_IOMUX_PAD_NAKED(2, 15, PAD_MUXSEL_0)
#define MX23_PAD_EMI_A07__EMI_A07		MXS_IOMUX_PAD_NAKED(2, 16, PAD_MUXSEL_0)
#define MX23_PAD_EMI_A08__EMI_A08		MXS_IOMUX_PAD_NAKED(2, 17, PAD_MUXSEL_0)
#define MX23_PAD_EMI_A09__EMI_A09		MXS_IOMUX_PAD_NAKED(2, 18, PAD_MUXSEL_0)
#define MX23_PAD_EMI_A10__EMI_A10		MXS_IOMUX_PAD_NAKED(2, 19, PAD_MUXSEL_0)
#define MX23_PAD_EMI_A11__EMI_A11		MXS_IOMUX_PAD_NAKED(2, 20, PAD_MUXSEL_0)
#define MX23_PAD_EMI_A12__EMI_A12		MXS_IOMUX_PAD_NAKED(2, 21, PAD_MUXSEL_0)
#define MX23_PAD_EMI_BA0__EMI_BA0		MXS_IOMUX_PAD_NAKED(2, 22, PAD_MUXSEL_0)
#define MX23_PAD_EMI_BA1__EMI_BA1		MXS_IOMUX_PAD_NAKED(2, 23, PAD_MUXSEL_0)
#define MX23_PAD_EMI_CASN__EMI_CASN		MXS_IOMUX_PAD_NAKED(2, 24, PAD_MUXSEL_0)
#define MX23_PAD_EMI_CE0N__EMI_CE0N		MXS_IOMUX_PAD_NAKED(2, 25, PAD_MUXSEL_0)
#define MX23_PAD_EMI_CE1N__EMI_CE1N		MXS_IOMUX_PAD_NAKED(2, 26, PAD_MUXSEL_0)
#define MX23_PAD_GPMI_CE1N__GPMI_CE1N		MXS_IOMUX_PAD_NAKED(2, 27, PAD_MUXSEL_0)
#define MX23_PAD_GPMI_CE0N__GPMI_CE0N		MXS_IOMUX_PAD_NAKED(2, 28, PAD_MUXSEL_0)
#define MX23_PAD_EMI_CKE__EMI_CKE		MXS_IOMUX_PAD_NAKED(2, 29, PAD_MUXSEL_0)
#define MX23_PAD_EMI_RASN__EMI_RASN		MXS_IOMUX_PAD_NAKED(2, 30, PAD_MUXSEL_0)
#define MX23_PAD_EMI_WEN__EMI_WEN		MXS_IOMUX_PAD_NAKED(2, 31, PAD_MUXSEL_0)

#define MX23_PAD_EMI_D00__EMI_D00		MXS_IOMUX_PAD_NAKED(3,  0, PAD_MUXSEL_0)
#define MX23_PAD_EMI_D01__EMI_D01		MXS_IOMUX_PAD_NAKED(3,  1, PAD_MUXSEL_0)
#define MX23_PAD_EMI_D02__EMI_D02		MXS_IOMUX_PAD_NAKED(3,  2, PAD_MUXSEL_0)
#define MX23_PAD_EMI_D03__EMI_D03		MXS_IOMUX_PAD_NAKED(3,  3, PAD_MUXSEL_0)
#define MX23_PAD_EMI_D04__EMI_D04		MXS_IOMUX_PAD_NAKED(3,  4, PAD_MUXSEL_0)
#define MX23_PAD_EMI_D05__EMI_D05		MXS_IOMUX_PAD_NAKED(3,  5, PAD_MUXSEL_0)
#define MX23_PAD_EMI_D06__EMI_D06		MXS_IOMUX_PAD_NAKED(3,  6, PAD_MUXSEL_0)
#define MX23_PAD_EMI_D07__EMI_D07		MXS_IOMUX_PAD_NAKED(3,  7, PAD_MUXSEL_0)
#define MX23_PAD_EMI_D08__EMI_D08		MXS_IOMUX_PAD_NAKED(3,  8, PAD_MUXSEL_0)
#define MX23_PAD_EMI_D09__EMI_D09		MXS_IOMUX_PAD_NAKED(3,  9, PAD_MUXSEL_0)
#define MX23_PAD_EMI_D10__EMI_D10		MXS_IOMUX_PAD_NAKED(3, 10, PAD_MUXSEL_0)
#define MX23_PAD_EMI_D11__EMI_D11		MXS_IOMUX_PAD_NAKED(3, 11, PAD_MUXSEL_0)
#define MX23_PAD_EMI_D12__EMI_D12		MXS_IOMUX_PAD_NAKED(3, 12, PAD_MUXSEL_0)
#define MX23_PAD_EMI_D13__EMI_D13		MXS_IOMUX_PAD_NAKED(3, 13, PAD_MUXSEL_0)
#define MX23_PAD_EMI_D14__EMI_D14		MXS_IOMUX_PAD_NAKED(3, 14, PAD_MUXSEL_0)
#define MX23_PAD_EMI_D15__EMI_D15		MXS_IOMUX_PAD_NAKED(3, 15, PAD_MUXSEL_0)
#define MX23_PAD_EMI_DQM0__EMI_DQM0		MXS_IOMUX_PAD_NAKED(3, 16, PAD_MUXSEL_0)
#define MX23_PAD_EMI_DQM1__EMI_DQM1		MXS_IOMUX_PAD_NAKED(3, 17, PAD_MUXSEL_0)
#define MX23_PAD_EMI_DQS0__EMI_DQS0		MXS_IOMUX_PAD_NAKED(3, 18, PAD_MUXSEL_0)
#define MX23_PAD_EMI_DQS1__EMI_DQS1		MXS_IOMUX_PAD_NAKED(3, 19, PAD_MUXSEL_0)
#define MX23_PAD_EMI_CLK__EMI_CLK		MXS_IOMUX_PAD_NAKED(3, 20, PAD_MUXSEL_0)
#define MX23_PAD_EMI_CLKN__EMI_CLKN		MXS_IOMUX_PAD_NAKED(3, 21, PAD_MUXSEL_0)

/* MUXSEL_1 */
#define MX23_PAD_GPMI_D00__LCD_D8		MXS_IOMUX_PAD_NAKED(0,  0, PAD_MUXSEL_1)
#define MX23_PAD_GPMI_D01__LCD_D9		MXS_IOMUX_PAD_NAKED(0,  1, PAD_MUXSEL_1)
#define MX23_PAD_GPMI_D02__LCD_D10		MXS_IOMUX_PAD_NAKED(0,  2, PAD_MUXSEL_1)
#define MX23_PAD_GPMI_D03__LCD_D11		MXS_IOMUX_PAD_NAKED(0,  3, PAD_MUXSEL_1)
#define MX23_PAD_GPMI_D04__LCD_D12		MXS_IOMUX_PAD_NAKED(0,  4, PAD_MUXSEL_1)
#define MX23_PAD_GPMI_D05__LCD_D13		MXS_IOMUX_PAD_NAKED(0,  5, PAD_MUXSEL_1)
#define MX23_PAD_GPMI_D06__LCD_D14		MXS_IOMUX_PAD_NAKED(0,  6, PAD_MUXSEL_1)
#define MX23_PAD_GPMI_D07__LCD_D15		MXS_IOMUX_PAD_NAKED(0,  7, PAD_MUXSEL_1)
#define MX23_PAD_GPMI_D08__LCD_D18		MXS_IOMUX_PAD_NAKED(0,  8, PAD_MUXSEL_1)
#define MX23_PAD_GPMI_D09__LCD_D19		MXS_IOMUX_PAD_NAKED(0,  9, PAD_MUXSEL_1)
#define MX23_PAD_GPMI_D10__LCD_D20		MXS_IOMUX_PAD_NAKED(0, 10, PAD_MUXSEL_1)
#define MX23_PAD_GPMI_D11__LCD_D21		MXS_IOMUX_PAD_NAKED(0, 11, PAD_MUXSEL_1)
#define MX23_PAD_GPMI_D12__LCD_D22		MXS_IOMUX_PAD_NAKED(0, 12, PAD_MUXSEL_1)
#define MX23_PAD_GPMI_D13__LCD_D23		MXS_IOMUX_PAD_NAKED(0, 13, PAD_MUXSEL_1)
#define MX23_PAD_GPMI_D14__AUART2_RX		MXS_IOMUX_PAD_NAKED(0, 14, PAD_MUXSEL_1)
#define MX23_PAD_GPMI_D15__AUART2_TX		MXS_IOMUX_PAD_NAKED(0, 15, PAD_MUXSEL_1)
#define MX23_PAD_GPMI_CLE__LCD_D16		MXS_IOMUX_PAD_NAKED(0, 16, PAD_MUXSEL_1)
#define MX23_PAD_GPMI_ALE__LCD_D17		MXS_IOMUX_PAD_NAKED(0, 17, PAD_MUXSEL_1)
#define MX23_PAD_GPMI_CE2N__ATA_A2		MXS_IOMUX_PAD_NAKED(0, 18, PAD_MUXSEL_1)
#define MX23_PAD_AUART1_RTS__IR_CLK		MXS_IOMUX_PAD_NAKED(0, 27, PAD_MUXSEL_1)
#define MX23_PAD_AUART1_RX__IR_RX		MXS_IOMUX_PAD_NAKED(0, 28, PAD_MUXSEL_1)
#define MX23_PAD_AUART1_TX__IR_TX		MXS_IOMUX_PAD_NAKED(0, 29, PAD_MUXSEL_1)
#define MX23_PAD_I2C_SCL__GPMI_RDY2		MXS_IOMUX_PAD_NAKED(0, 30, PAD_MUXSEL_1)
#define MX23_PAD_I2C_SDA__GPMI_CE2N		MXS_IOMUX_PAD_NAKED(0, 31, PAD_MUXSEL_1)

#define MX23_PAD_LCD_D00__ETM_DA8		MXS_IOMUX_PAD_NAKED(1,  0, PAD_MUXSEL_1)
#define MX23_PAD_LCD_D01__ETM_DA9		MXS_IOMUX_PAD_NAKED(1,  1, PAD_MUXSEL_1)
#define MX23_PAD_LCD_D02__ETM_DA10		MXS_IOMUX_PAD_NAKED(1,  2, PAD_MUXSEL_1)
#define MX23_PAD_LCD_D03__ETM_DA11		MXS_IOMUX_PAD_NAKED(1,  3, PAD_MUXSEL_1)
#define MX23_PAD_LCD_D04__ETM_DA12		MXS_IOMUX_PAD_NAKED(1,  4, PAD_MUXSEL_1)
#define MX23_PAD_LCD_D05__ETM_DA13		MXS_IOMUX_PAD_NAKED(1,  5, PAD_MUXSEL_1)
#define MX23_PAD_LCD_D06__ETM_DA14		MXS_IOMUX_PAD_NAKED(1,  6, PAD_MUXSEL_1)
#define MX23_PAD_LCD_D07__ETM_DA15		MXS_IOMUX_PAD_NAKED(1,  7, PAD_MUXSEL_1)
#define MX23_PAD_LCD_D08__ETM_DA0		MXS_IOMUX_PAD_NAKED(1,  8, PAD_MUXSEL_1)
#define MX23_PAD_LCD_D09__ETM_DA1		MXS_IOMUX_PAD_NAKED(1,  9, PAD_MUXSEL_1)
#define MX23_PAD_LCD_D10__ETM_DA2		MXS_IOMUX_PAD_NAKED(1, 10, PAD_MUXSEL_1)
#define MX23_PAD_LCD_D11__ETM_DA3		MXS_IOMUX_PAD_NAKED(1, 11, PAD_MUXSEL_1)
#define MX23_PAD_LCD_D12__ETM_DA4		MXS_IOMUX_PAD_NAKED(1, 12, PAD_MUXSEL_1)
#define MX23_PAD_LCD_D13__ETM_DA5		MXS_IOMUX_PAD_NAKED(1, 13, PAD_MUXSEL_1)
#define MX23_PAD_LCD_D14__ETM_DA6		MXS_IOMUX_PAD_NAKED(1, 14, PAD_MUXSEL_1)
#define MX23_PAD_LCD_D15__ETM_DA7		MXS_IOMUX_PAD_NAKED(1, 15, PAD_MUXSEL_1)
#define MX23_PAD_LCD_RESET__ETM_TCTL		MXS_IOMUX_PAD_NAKED(1, 18, PAD_MUXSEL_1)
#define MX23_PAD_LCD_RS__ETM_TCLK		MXS_IOMUX_PAD_NAKED(1, 19, PAD_MUXSEL_1)
#define MX23_PAD_LCD_DOTCK__GPMI_RDY3		MXS_IOMUX_PAD_NAKED(1, 22, PAD_MUXSEL_1)
#define MX23_PAD_LCD_ENABLE__I2C_SCL		MXS_IOMUX_PAD_NAKED(1, 23, PAD_MUXSEL_1)
#define MX23_PAD_LCD_HSYNC__I2C_SDA		MXS_IOMUX_PAD_NAKED(1, 24, PAD_MUXSEL_1)
#define MX23_PAD_LCD_VSYNC__LCD_BUSY		MXS_IOMUX_PAD_NAKED(1, 25, PAD_MUXSEL_1)
#define MX23_PAD_PWM0__ROTARYA			MXS_IOMUX_PAD_NAKED(1, 26, PAD_MUXSEL_1)
#define MX23_PAD_PWM1__ROTARYB			MXS_IOMUX_PAD_NAKED(1, 27, PAD_MUXSEL_1)
#define MX23_PAD_PWM2__GPMI_RDY3		MXS_IOMUX_PAD_NAKED(1, 28, PAD_MUXSEL_1)
#define MX23_PAD_PWM3__ETM_TCTL			MXS_IOMUX_PAD_NAKED(1, 29, PAD_MUXSEL_1)
#define MX23_PAD_PWM4__ETM_TCLK			MXS_IOMUX_PAD_NAKED(1, 30, PAD_MUXSEL_1)

#define MX23_PAD_SSP1_DETECT__GPMI_CE3N		MXS_IOMUX_PAD_NAKED(2,  1, PAD_MUXSEL_1)
#define MX23_PAD_SSP1_DATA1__I2C_SCL		MXS_IOMUX_PAD_NAKED(2,  3, PAD_MUXSEL_1)
#define MX23_PAD_SSP1_DATA2__I2C_SDA		MXS_IOMUX_PAD_NAKED(2,  4, PAD_MUXSEL_1)
#define MX23_PAD_ROTARYA__AUART2_RTS		MXS_IOMUX_PAD_NAKED(2,  7, PAD_MUXSEL_1)
#define MX23_PAD_ROTARYB__AUART2_CTS		MXS_IOMUX_PAD_NAKED(2,  8, PAD_MUXSEL_1)

/* MUXSEL_2 */
#define MX23_PAD_GPMI_D00__SSP2_DATA0		MXS_IOMUX_PAD_NAKED(0,  0, PAD_MUXSEL_2)
#define MX23_PAD_GPMI_D01__SSP2_DATA1		MXS_IOMUX_PAD_NAKED(0,  1, PAD_MUXSEL_2)
#define MX23_PAD_GPMI_D02__SSP2_DATA2		MXS_IOMUX_PAD_NAKED(0,  2, PAD_MUXSEL_2)
#define MX23_PAD_GPMI_D03__SSP2_DATA3		MXS_IOMUX_PAD_NAKED(0,  3, PAD_MUXSEL_2)
#define MX23_PAD_GPMI_D04__SSP2_DATA4		MXS_IOMUX_PAD_NAKED(0,  4, PAD_MUXSEL_2)
#define MX23_PAD_GPMI_D05__SSP2_DATA5		MXS_IOMUX_PAD_NAKED(0,  5, PAD_MUXSEL_2)
#define MX23_PAD_GPMI_D06__SSP2_DATA6		MXS_IOMUX_PAD_NAKED(0,  6, PAD_MUXSEL_2)
#define MX23_PAD_GPMI_D07__SSP2_DATA7		MXS_IOMUX_PAD_NAKED(0,  7, PAD_MUXSEL_2)
#define MX23_PAD_GPMI_D08__SSP1_DATA4		MXS_IOMUX_PAD_NAKED(0,  8, PAD_MUXSEL_2)
#define MX23_PAD_GPMI_D09__SSP1_DATA5		MXS_IOMUX_PAD_NAKED(0,  9, PAD_MUXSEL_2)
#define MX23_PAD_GPMI_D10__SSP1_DATA6		MXS_IOMUX_PAD_NAKED(0, 10, PAD_MUXSEL_2)
#define MX23_PAD_GPMI_D11__SSP1_DATA7		MXS_IOMUX_PAD_NAKED(0, 11, PAD_MUXSEL_2)
#define MX23_PAD_GPMI_D15__GPMI_CE3N		MXS_IOMUX_PAD_NAKED(0, 15, PAD_MUXSEL_2)
#define MX23_PAD_GPMI_RDY0__SSP2_DETECT		MXS_IOMUX_PAD_NAKED(0, 19, PAD_MUXSEL_2)
#define MX23_PAD_GPMI_RDY1__SSP2_CMD		MXS_IOMUX_PAD_NAKED(0, 20, PAD_MUXSEL_2)
#define MX23_PAD_GPMI_WRN__SSP2_SCK		MXS_IOMUX_PAD_NAKED(0, 24, PAD_MUXSEL_2)
#define MX23_PAD_AUART1_CTS__SSP1_DATA4		MXS_IOMUX_PAD_NAKED(0, 26, PAD_MUXSEL_2)
#define MX23_PAD_AUART1_RTS__SSP1_DATA5		MXS_IOMUX_PAD_NAKED(0, 27, PAD_MUXSEL_2)
#define MX23_PAD_AUART1_RX__SSP1_DATA6		MXS_IOMUX_PAD_NAKED(0, 28, PAD_MUXSEL_2)
#define MX23_PAD_AUART1_TX__SSP1_DATA7		MXS_IOMUX_PAD_NAKED(0, 29, PAD_MUXSEL_2)
#define MX23_PAD_I2C_SCL__AUART1_TX		MXS_IOMUX_PAD_NAKED(0, 30, PAD_MUXSEL_2)
#define MX23_PAD_I2C_SDA__AUART1_RX		MXS_IOMUX_PAD_NAKED(0, 31, PAD_MUXSEL_2)

#define MX23_PAD_LCD_D08__SAIF2_SDATA0		MXS_IOMUX_PAD_NAKED(1,  8, PAD_MUXSEL_2)
#define MX23_PAD_LCD_D09__SAIF1_SDATA0		MXS_IOMUX_PAD_NAKED(1,  9, PAD_MUXSEL_2)
#define MX23_PAD_LCD_D10__SAIF_MCLK_BITCLK	MXS_IOMUX_PAD_NAKED(1, 10, PAD_MUXSEL_2)
#define MX23_PAD_LCD_D11__SAIF_LRCLK		MXS_IOMUX_PAD_NAKED(1, 11, PAD_MUXSEL_2)
#define MX23_PAD_LCD_D12__SAIF2_SDATA1		MXS_IOMUX_PAD_NAKED(1, 12, PAD_MUXSEL_2)
#define MX23_PAD_LCD_D13__SAIF2_SDATA2		MXS_IOMUX_PAD_NAKED(1, 13, PAD_MUXSEL_2)
#define MX23_PAD_LCD_D14__SAIF1_SDATA2		MXS_IOMUX_PAD_NAKED(1, 14, PAD_MUXSEL_2)
#define MX23_PAD_LCD_D15__SAIF1_SDATA1		MXS_IOMUX_PAD_NAKED(1, 15, PAD_MUXSEL_2)
#define MX23_PAD_LCD_D16__SAIF_ALT_BITCLK	MXS_IOMUX_PAD_NAKED(1, 16, PAD_MUXSEL_2)
#define MX23_PAD_LCD_RESET__GPMI_CE3N		MXS_IOMUX_PAD_NAKED(1, 18, PAD_MUXSEL_2)
#define MX23_PAD_PWM0__DUART_RX			MXS_IOMUX_PAD_NAKED(1, 26, PAD_MUXSEL_2)
#define MX23_PAD_PWM1__DUART_TX			MXS_IOMUX_PAD_NAKED(1, 27, PAD_MUXSEL_2)
#define MX23_PAD_PWM3__AUART1_CTS		MXS_IOMUX_PAD_NAKED(1, 29, PAD_MUXSEL_2)
#define MX23_PAD_PWM4__AUART1_RTS		MXS_IOMUX_PAD_NAKED(1, 30, PAD_MUXSEL_2)

#define MX23_PAD_SSP1_CMD__JTAG_TDO		MXS_IOMUX_PAD_NAKED(2,  0, PAD_MUXSEL_2)
#define MX23_PAD_SSP1_DETECT__USB_OTG_ID	MXS_IOMUX_PAD_NAKED(2,  1, PAD_MUXSEL_2)
#define MX23_PAD_SSP1_DATA0__JTAG_TDI		MXS_IOMUX_PAD_NAKED(2,  2, PAD_MUXSEL_2)
#define MX23_PAD_SSP1_DATA1__JTAG_TCLK		MXS_IOMUX_PAD_NAKED(2,  3, PAD_MUXSEL_2)
#define MX23_PAD_SSP1_DATA2__JTAG_RTCK		MXS_IOMUX_PAD_NAKED(2,  4, PAD_MUXSEL_2)
#define MX23_PAD_SSP1_DATA3__JTAG_TMS		MXS_IOMUX_PAD_NAKED(2,  5, PAD_MUXSEL_2)
#define MX23_PAD_SSP1_SCK__JTAG_TRST		MXS_IOMUX_PAD_NAKED(2,  6, PAD_MUXSEL_2)
#define MX23_PAD_ROTARYA__SPDIF			MXS_IOMUX_PAD_NAKED(2,  7, PAD_MUXSEL_2)
#define MX23_PAD_ROTARYB__GPMI_CE3N		MXS_IOMUX_PAD_NAKED(2,  8, PAD_MUXSEL_2)

/* MUXSEL_GPIO */
#define MX23_PAD_GPMI_D00__GPIO_0_0		MXS_IOMUX_PAD_NAKED(0,  0, PAD_MUXSEL_GPIO)
#define MX23_PAD_GPMI_D01__GPIO_0_1		MXS_IOMUX_PAD_NAKED(0,  1, PAD_MUXSEL_GPIO)
#define MX23_PAD_GPMI_D02__GPIO_0_2		MXS_IOMUX_PAD_NAKED(0,  2, PAD_MUXSEL_GPIO)
#define MX23_PAD_GPMI_D03__GPIO_0_3		MXS_IOMUX_PAD_NAKED(0,  3, PAD_MUXSEL_GPIO)
#define MX23_PAD_GPMI_D04__GPIO_0_4		MXS_IOMUX_PAD_NAKED(0,  4, PAD_MUXSEL_GPIO)
#define MX23_PAD_GPMI_D05__GPIO_0_5		MXS_IOMUX_PAD_NAKED(0,  5, PAD_MUXSEL_GPIO)
#define MX23_PAD_GPMI_D06__GPIO_0_6		MXS_IOMUX_PAD_NAKED(0,  6, PAD_MUXSEL_GPIO)
#define MX23_PAD_GPMI_D07__GPIO_0_7		MXS_IOMUX_PAD_NAKED(0,  7, PAD_MUXSEL_GPIO)
#define MX23_PAD_GPMI_D08__GPIO_0_8		MXS_IOMUX_PAD_NAKED(0,  8, PAD_MUXSEL_GPIO)
#define MX23_PAD_GPMI_D09__GPIO_0_9		MXS_IOMUX_PAD_NAKED(0,  9, PAD_MUXSEL_GPIO)
#define MX23_PAD_GPMI_D10__GPIO_0_10		MXS_IOMUX_PAD_NAKED(0, 10, PAD_MUXSEL_GPIO)
#define MX23_PAD_GPMI_D11__GPIO_0_11		MXS_IOMUX_PAD_NAKED(0, 11, PAD_MUXSEL_GPIO)
#define MX23_PAD_GPMI_D12__GPIO_0_12		MXS_IOMUX_PAD_NAKED(0, 12, PAD_MUXSEL_GPIO)
#define MX23_PAD_GPMI_D13__GPIO_0_13		MXS_IOMUX_PAD_NAKED(0, 13, PAD_MUXSEL_GPIO)
#define MX23_PAD_GPMI_D14__GPIO_0_14		MXS_IOMUX_PAD_NAKED(0, 14, PAD_MUXSEL_GPIO)
#define MX23_PAD_GPMI_D15__GPIO_0_15		MXS_IOMUX_PAD_NAKED(0, 15, PAD_MUXSEL_GPIO)
#define MX23_PAD_GPMI_CLE__GPIO_0_16		MXS_IOMUX_PAD_NAKED(0, 16, PAD_MUXSEL_GPIO)
#define MX23_PAD_GPMI_ALE__GPIO_0_17		MXS_IOMUX_PAD_NAKED(0, 17, PAD_MUXSEL_GPIO)
#define MX23_PAD_GPMI_CE2N__GPIO_0_18		MXS_IOMUX_PAD_NAKED(0, 18, PAD_MUXSEL_GPIO)
#define MX23_PAD_GPMI_RDY0__GPIO_0_19		MXS_IOMUX_PAD_NAKED(0, 19, PAD_MUXSEL_GPIO)
#define MX23_PAD_GPMI_RDY1__GPIO_0_20		MXS_IOMUX_PAD_NAKED(0, 20, PAD_MUXSEL_GPIO)
#define MX23_PAD_GPMI_RDY2__GPIO_0_21		MXS_IOMUX_PAD_NAKED(0, 21, PAD_MUXSEL_GPIO)
#define MX23_PAD_GPMI_RDY3__GPIO_0_22		MXS_IOMUX_PAD_NAKED(0, 22, PAD_MUXSEL_GPIO)
#define MX23_PAD_GPMI_WPN__GPIO_0_23		MXS_IOMUX_PAD_NAKED(0, 23, PAD_MUXSEL_GPIO)
#define MX23_PAD_GPMI_WRN__GPIO_0_24		MXS_IOMUX_PAD_NAKED(0, 24, PAD_MUXSEL_GPIO)
#define MX23_PAD_GPMI_RDN__GPIO_0_25		MXS_IOMUX_PAD_NAKED(0, 25, PAD_MUXSEL_GPIO)
#define MX23_PAD_AUART1_CTS__GPIO_0_26		MXS_IOMUX_PAD_NAKED(0, 26, PAD_MUXSEL_GPIO)
#define MX23_PAD_AUART1_RTS__GPIO_0_27		MXS_IOMUX_PAD_NAKED(0, 27, PAD_MUXSEL_GPIO)
#define MX23_PAD_AUART1_RX__GPIO_0_28		MXS_IOMUX_PAD_NAKED(0, 28, PAD_MUXSEL_GPIO)
#define MX23_PAD_AUART1_TX__GPIO_0_29		MXS_IOMUX_PAD_NAKED(0, 29, PAD_MUXSEL_GPIO)
#define MX23_PAD_I2C_SCL__GPIO_0_30		MXS_IOMUX_PAD_NAKED(0, 30, PAD_MUXSEL_GPIO)
#define MX23_PAD_I2C_SDA__GPIO_0_31		MXS_IOMUX_PAD_NAKED(0, 31, PAD_MUXSEL_GPIO)

#define MX23_PAD_LCD_D00__GPIO_1_0		MXS_IOMUX_PAD_NAKED(1,  0, PAD_MUXSEL_GPIO)
#define MX23_PAD_LCD_D01__GPIO_1_1		MXS_IOMUX_PAD_NAKED(1,  1, PAD_MUXSEL_GPIO)
#define MX23_PAD_LCD_D02__GPIO_1_2		MXS_IOMUX_PAD_NAKED(1,  2, PAD_MUXSEL_GPIO)
#define MX23_PAD_LCD_D03__GPIO_1_3		MXS_IOMUX_PAD_NAKED(1,  3, PAD_MUXSEL_GPIO)
#define MX23_PAD_LCD_D04__GPIO_1_4		MXS_IOMUX_PAD_NAKED(1,  4, PAD_MUXSEL_GPIO)
#define MX23_PAD_LCD_D05__GPIO_1_5		MXS_IOMUX_PAD_NAKED(1,  5, PAD_MUXSEL_GPIO)
#define MX23_PAD_LCD_D06__GPIO_1_6		MXS_IOMUX_PAD_NAKED(1,  6, PAD_MUXSEL_GPIO)
#define MX23_PAD_LCD_D07__GPIO_1_7		MXS_IOMUX_PAD_NAKED(1,  7, PAD_MUXSEL_GPIO)
#define MX23_PAD_LCD_D08__GPIO_1_8		MXS_IOMUX_PAD_NAKED(1,  8, PAD_MUXSEL_GPIO)
#define MX23_PAD_LCD_D09__GPIO_1_9		MXS_IOMUX_PAD_NAKED(1,  9, PAD_MUXSEL_GPIO)
#define MX23_PAD_LCD_D10__GPIO_1_10		MXS_IOMUX_PAD_NAKED(1, 10, PAD_MUXSEL_GPIO)
#define MX23_PAD_LCD_D11__GPIO_1_11		MXS_IOMUX_PAD_NAKED(1, 11, PAD_MUXSEL_GPIO)
#define MX23_PAD_LCD_D12__GPIO_1_12		MXS_IOMUX_PAD_NAKED(1, 12, PAD_MUXSEL_GPIO)
#define MX23_PAD_LCD_D13__GPIO_1_13		MXS_IOMUX_PAD_NAKED(1, 13, PAD_MUXSEL_GPIO)
#define MX23_PAD_LCD_D14__GPIO_1_14		MXS_IOMUX_PAD_NAKED(1, 14, PAD_MUXSEL_GPIO)
#define MX23_PAD_LCD_D15__GPIO_1_15		MXS_IOMUX_PAD_NAKED(1, 15, PAD_MUXSEL_GPIO)
#define MX23_PAD_LCD_D16__GPIO_1_16		MXS_IOMUX_PAD_NAKED(1, 16, PAD_MUXSEL_GPIO)
#define MX23_PAD_LCD_D17__GPIO_1_17		MXS_IOMUX_PAD_NAKED(1, 17, PAD_MUXSEL_GPIO)
#define MX23_PAD_LCD_RESET__GPIO_1_18		MXS_IOMUX_PAD_NAKED(1, 18, PAD_MUXSEL_GPIO)
#define MX23_PAD_LCD_RS__GPIO_1_19		MXS_IOMUX_PAD_NAKED(1, 19, PAD_MUXSEL_GPIO)
#define MX23_PAD_LCD_WR__GPIO_1_20		MXS_IOMUX_PAD_NAKED(1, 20, PAD_MUXSEL_GPIO)
#define MX23_PAD_LCD_CS__GPIO_1_21		MXS_IOMUX_PAD_NAKED(1, 21, PAD_MUXSEL_GPIO)
#define MX23_PAD_LCD_DOTCK__GPIO_1_22		MXS_IOMUX_PAD_NAKED(1, 22, PAD_MUXSEL_GPIO)
#define MX23_PAD_LCD_ENABLE__GPIO_1_23		MXS_IOMUX_PAD_NAKED(1, 23, PAD_MUXSEL_GPIO)
#define MX23_PAD_LCD_HSYNC__GPIO_1_24		MXS_IOMUX_PAD_NAKED(1, 24, PAD_MUXSEL_GPIO)
#define MX23_PAD_LCD_VSYNC__GPIO_1_25		MXS_IOMUX_PAD_NAKED(1, 25, PAD_MUXSEL_GPIO)
#define MX23_PAD_PWM0__GPIO_1_26		MXS_IOMUX_PAD_NAKED(1, 26, PAD_MUXSEL_GPIO)
#define MX23_PAD_PWM1__GPIO_1_27		MXS_IOMUX_PAD_NAKED(1, 27, PAD_MUXSEL_GPIO)
#define MX23_PAD_PWM2__GPIO_1_28		MXS_IOMUX_PAD_NAKED(1, 28, PAD_MUXSEL_GPIO)
#define MX23_PAD_PWM3__GPIO_1_29		MXS_IOMUX_PAD_NAKED(1, 29, PAD_MUXSEL_GPIO)
#define MX23_PAD_PWM4__GPIO_1_30		MXS_IOMUX_PAD_NAKED(1, 30, PAD_MUXSEL_GPIO)

#define MX23_PAD_SSP1_CMD__GPIO_2_0		MXS_IOMUX_PAD_NAKED(2,  0, PAD_MUXSEL_GPIO)
#define MX23_PAD_SSP1_DETECT__GPIO_2_1		MXS_IOMUX_PAD_NAKED(2,  1, PAD_MUXSEL_GPIO)
#define MX23_PAD_SSP1_DATA0__GPIO_2_2		MXS_IOMUX_PAD_NAKED(2,  2, PAD_MUXSEL_GPIO)
#define MX23_PAD_SSP1_DATA1__GPIO_2_3		MXS_IOMUX_PAD_NAKED(2,  3, PAD_MUXSEL_GPIO)
#define MX23_PAD_SSP1_DATA2__GPIO_2_4		MXS_IOMUX_PAD_NAKED(2,  4, PAD_MUXSEL_GPIO)
#define MX23_PAD_SSP1_DATA3__GPIO_2_5		MXS_IOMUX_PAD_NAKED(2,  5, PAD_MUXSEL_GPIO)
#define MX23_PAD_SSP1_SCK__GPIO_2_6		MXS_IOMUX_PAD_NAKED(2,  6, PAD_MUXSEL_GPIO)
#define MX23_PAD_ROTARYA__GPIO_2_7		MXS_IOMUX_PAD_NAKED(2,  7, PAD_MUXSEL_GPIO)
#define MX23_PAD_ROTARYB__GPIO_2_8		MXS_IOMUX_PAD_NAKED(2,  8, PAD_MUXSEL_GPIO)
#define MX23_PAD_EMI_A00__GPIO_2_9		MXS_IOMUX_PAD_NAKED(2,  9, PAD_MUXSEL_GPIO)
#define MX23_PAD_EMI_A01__GPIO_2_10		MXS_IOMUX_PAD_NAKED(2, 10, PAD_MUXSEL_GPIO)
#define MX23_PAD_EMI_A02__GPIO_2_11		MXS_IOMUX_PAD_NAKED(2, 11, PAD_MUXSEL_GPIO)
#define MX23_PAD_EMI_A03__GPIO_2_12		MXS_IOMUX_PAD_NAKED(2, 12, PAD_MUXSEL_GPIO)
#define MX23_PAD_EMI_A04__GPIO_2_13		MXS_IOMUX_PAD_NAKED(2, 13, PAD_MUXSEL_GPIO)
#define MX23_PAD_EMI_A05__GPIO_2_14		MXS_IOMUX_PAD_NAKED(2, 14, PAD_MUXSEL_GPIO)
#define MX23_PAD_EMI_A06__GPIO_2_15		MXS_IOMUX_PAD_NAKED(2, 15, PAD_MUXSEL_GPIO)
#define MX23_PAD_EMI_A07__GPIO_2_16		MXS_IOMUX_PAD_NAKED(2, 16, PAD_MUXSEL_GPIO)
#define MX23_PAD_EMI_A08__GPIO_2_17		MXS_IOMUX_PAD_NAKED(2, 17, PAD_MUXSEL_GPIO)
#define MX23_PAD_EMI_A09__GPIO_2_18		MXS_IOMUX_PAD_NAKED(2, 18, PAD_MUXSEL_GPIO)
#define MX23_PAD_EMI_A10__GPIO_2_19		MXS_IOMUX_PAD_NAKED(2, 19, PAD_MUXSEL_GPIO)
#define MX23_PAD_EMI_A11__GPIO_2_20		MXS_IOMUX_PAD_NAKED(2, 20, PAD_MUXSEL_GPIO)
#define MX23_PAD_EMI_A12__GPIO_2_21		MXS_IOMUX_PAD_NAKED(2, 21, PAD_MUXSEL_GPIO)
#define MX23_PAD_EMI_BA0__GPIO_2_22		MXS_IOMUX_PAD_NAKED(2, 22, PAD_MUXSEL_GPIO)
#define MX23_PAD_EMI_BA1__GPIO_2_23		MXS_IOMUX_PAD_NAKED(2, 23, PAD_MUXSEL_GPIO)
#define MX23_PAD_EMI_CASN__GPIO_2_24		MXS_IOMUX_PAD_NAKED(2, 24, PAD_MUXSEL_GPIO)
#define MX23_PAD_EMI_CE0N__GPIO_2_25		MXS_IOMUX_PAD_NAKED(2, 25, PAD_MUXSEL_GPIO)
#define MX23_PAD_EMI_CE1N__GPIO_2_26		MXS_IOMUX_PAD_NAKED(2, 26, PAD_MUXSEL_GPIO)
#define MX23_PAD_GPMI_CE1N__GPIO_2_27		MXS_IOMUX_PAD_NAKED(2, 27, PAD_MUXSEL_GPIO)
#define MX23_PAD_GPMI_CE0N__GPIO_2_28		MXS_IOMUX_PAD_NAKED(2, 28, PAD_MUXSEL_GPIO)
#define MX23_PAD_EMI_CKE__GPIO_2_29		MXS_IOMUX_PAD_NAKED(2, 29, PAD_MUXSEL_GPIO)
#define MX23_PAD_EMI_RASN__GPIO_2_30		MXS_IOMUX_PAD_NAKED(2, 30, PAD_MUXSEL_GPIO)
#define MX23_PAD_EMI_WEN__GPIO_2_31		MXS_IOMUX_PAD_NAKED(2, 31, PAD_MUXSEL_GPIO)

#endif /* __MACH_IOMUX_MX23_H__ */
