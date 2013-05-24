#ifndef __LINUX_JZ_ADC_H__
#define __LINUX_JZ_ADC_H__

#include <linux/device.h>

/*
 * jz_adc_set_config - Configure a JZ4780 adc device
 * @dev: Pointer to a jz4780-adc device
 * @mask: Mask for the config value to be set
 * @val: Value to be set
 *
 * This function can be used by the JZ4780
 * ADC mfd cells to confgure their options in the shared config register.
 */

int jz_adc_set_config(struct device *dev, uint32_t mask, uint32_t val);

#endif /*jz_adc.h*/
