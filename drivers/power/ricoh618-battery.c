/*
 * drivers/power/ricoh618-battery.c
 *
 * Charger driver for RICOH RN5T618 power management chip.
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
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/mutex.h>
#include <linux/string.h>
#include <linux/power_supply.h>
#include <linux/mfd/ricoh618.h>
#include <linux/mfd/pmu-common.h>
#include <linux/power/ricoh618_battery.h>
#include <linux/power/ricoh61x_battery_init.h>
#include <linux/delay.h>
#include <linux/workqueue.h>

//define for function
//if FG function is enabled, please define below item.
//#define FUEL_GAGE_FUNCTION_ENABLE		

struct ricoh618_battery_info {
	struct device      *dev;
	struct power_supply	battery;
	struct power_supply	ac;
	struct power_supply	usb;
	struct delayed_work	monitor_work;
	struct delayed_work	changed_work;
	struct workqueue_struct * monitor_wqueue;
	struct mutex		lock;
	unsigned long       monitor_time;
	int		irq_ac;
	int		irq_usb;
	int		adc_vdd_mv;
	int		multiple;
	int		alarm_vol_mv;
	int		status;
	int		min_voltage;
	int		max_voltage;
	int		cur_voltage;
	int		capacity;
	int		battery_temp;
	int		time_to_empty;
	int		time_to_full;
	unsigned	present : 1;
};

static void ricoh618_battery_work(struct work_struct *work)
{
	struct ricoh618_battery_info *info = container_of(work,
	struct ricoh618_battery_info, monitor_work.work);

	power_supply_changed(&info->battery);
	queue_delayed_work(info->monitor_wqueue, &info->monitor_work,
			   info->monitor_time);
}

static void ricoh618_changed_work(struct work_struct *work)
{
	struct ricoh618_battery_info *info = container_of(work,
	struct ricoh618_battery_info, changed_work.work);

	power_supply_changed(&info->battery);

	return;
}

//Initial setting of battery
static int ricoh618_init_battery(struct ricoh618_battery_info *info)
{
	int ret = 0;
      //Need to implement initial setting of batery and error
      //////////////////////////////
#ifdef FUEL_GAGE_FUNCTION_ENABLE
	uint8_t val = 0;

	ret = ricoh618_read(info->dev->parent, FG_CTRL_REG, &val);
	if(ret<0){
		dev_err(info->dev, "Error in reading the control register\n");
		return ret;
	}
	
	val = (val & 0x10) >> 4;//check FG_ACC DATA
	
	if(val == 0)
	{//set initial setting of battery
		ret = ricoh618_bulk_writes_bank1(info->dev->parent, BAT_INIT_TOP_REG, 32, battery_init_para);	
		if(ret<0){
			dev_err(info->dev, "batterry initialize error\n");
			return ret;
		}

	}
	
	ret = ricoh618_write(info->dev->parent, FG_CTRL_REG, 0x11);
	if(ret<0){
		dev_err(info->dev, "Error in writing the control register\n");
		return ret;
	}

#endif
      
      //////////////////////////////

	if(info->alarm_vol_mv < 2700 || info->alarm_vol_mv > 3400){
		dev_err(info->dev, "alarm_vol_mv is out of range!\n");
		return -1;
	}

	return ret;
}


static int get_power_supply_status(struct ricoh618_battery_info *info)
{
	uint8_t status;
	uint8_t supply_state;
	uint8_t charge_state;
	int temp;
	int ret = 0;

	//get  power supply status
	ret = ricoh618_read(info->dev->parent, CHGSTATE_REG, &status);
	if(ret<0){
		dev_err(info->dev, "Error in reading the control register\n");
		return ret;
	}

	charge_state = (status & 0x1F);
	supply_state = (status & 0xC0);

	if (supply_state == SUPPLY_STATE_BAT)
	{
		temp = POWER_SUPPLY_STATUS_DISCHARGING;
	}else{
		switch (charge_state){
		case	CHG_STATE_CHG_OFF:
				temp = POWER_SUPPLY_STATUS_DISCHARGING;
				break;
		case	CHG_STATE_CHG_READY_VADP:
				temp = POWER_SUPPLY_STATUS_NOT_CHARGING;
				break;
		case	CHG_STATE_CHG_TRICKLE:
				temp = POWER_SUPPLY_STATUS_CHARGING;
				break;
		case	CHG_STATE_CHG_RAPID:
				temp = POWER_SUPPLY_STATUS_CHARGING;
				break;
		case	CHG_STATE_CHG_COMPLETE:
				temp = POWER_SUPPLY_STATUS_FULL;
				break;
		case	CHG_STATE_SUSPEND:
				temp = POWER_SUPPLY_STATUS_DISCHARGING;
				break;
		case	CHG_STATE_VCHG_OVER_VOL:
				temp = POWER_SUPPLY_STATUS_DISCHARGING;
				break;
		case	CHG_STATE_BAT_ERROR:
				temp = POWER_SUPPLY_STATUS_NOT_CHARGING;
				break;
		case	CHG_STATE_NO_BAT:
				temp = POWER_SUPPLY_STATUS_NOT_CHARGING;
				break;
		case	CHG_STATE_BAT_OVER_VOL:
				temp = POWER_SUPPLY_STATUS_NOT_CHARGING;
				break;
		case	CHG_STATE_BAT_TEMP_ERR:
				temp = POWER_SUPPLY_STATUS_NOT_CHARGING;
				break;
		case	CHG_STATE_DIE_ERR:
				temp = POWER_SUPPLY_STATUS_NOT_CHARGING;
				break;
		case	CHG_STATE_DIE_SHUTDOWN:
				temp = POWER_SUPPLY_STATUS_DISCHARGING;
				break;
		case	CHG_STATE_NO_BAT2:
				temp = POWER_SUPPLY_STATUS_NOT_CHARGING;
				break;
		case	CHG_STATE_CHG_READY_VUSB:
				temp = POWER_SUPPLY_STATUS_NOT_CHARGING;
				break;
		default:
				temp = POWER_SUPPLY_STATUS_UNKNOWN;
				break;
		}
	}
	
	return temp;
} 



#ifdef	FUEL_GAGE_FUNCTION_ENABLE
static int get_check_fuel_gauge_reg(struct ricoh618_battery_info * info, int Reg_h, int Reg_l, int enable_bit)
{
	uint8_t get_data_h, get_data_l;
	int old_data, current_data;
	int i;
	int ret = 0;
	
	old_data = 0;
	
	for( i = 0; i < 5 ; i++)
	{
		ret = ricoh618_read(info->dev->parent, Reg_h, &get_data_h);
		if (ret < 0){
			dev_err(info->dev, "Error in reading the control register\n");
			return ret;
		}

		ret = ricoh618_read(info->dev->parent, Reg_l, &get_data_l);
		if (ret < 0){
			dev_err(info->dev, "Error in reading the control register\n");
			return ret;
		}
		
		current_data =((get_data_h & 0xff) << 8) | (get_data_l & 0xff);
		current_data = (current_data & enable_bit);
		
		if(current_data == old_data)
		{
			return current_data;
		}else{
			
			old_data = current_data;
		}
	}
	
	return current_data;
}	

static int calc_capacity(struct ricoh618_battery_info *info)
{
	uint8_t capacity;
	int temp;
	int ret = 0;
	
	//get remaining battery capacity from fuel gauge
	ret = ricoh618_read(info->dev->parent, SOC_REG, &capacity);
	if(ret<0){
		dev_err(info->dev, "Error in reading the control register\n");
		return ret;
	}

	temp = capacity;

	return temp;		//Unit is 1%
}

static int get_buttery_temp(struct ricoh618_battery_info *info)
{
	uint8_t sign_bit;
	int ret = 0;

	ret = get_check_fuel_gauge_reg(info, TEMP_1_REG, TEMP_2_REG, 0x0fff);
	if(ret<0){
		dev_err(info->dev, "Error in reading the fuel gauge control register\n");
		return ret;
	}

	ret = ( ret & 0x07ff);
	
	ret = ricoh618_read(info->dev->parent, TEMP_1_REG, &sign_bit);
	if(ret<0){
		dev_err(info->dev, "Error in reading the control register\n");
		return ret;
	}

	sign_bit = ( (sign_bit & 0x08) >> 3 ); 							//bit3 of 0xED(TEMP_1) is sign_bit
	
	if(sign_bit ==0){ //positive value part
		ret = ret * 625  / 1000;						//conversion unit // 1 unit is 0.0625 degree and retun unit should be 0.1 degree,
	}else//negative value part
	{
		ret = -1 * ret * 625 /1000;
	}

	return ret;
}

static int get_time_to_empty(struct ricoh618_battery_info *info)
{
	int ret = 0;

	ret = get_check_fuel_gauge_reg(info, TT_EMPTY_H_REG, TT_EMPTY_L_REG, 0xffff);
	if(ret<0){
		dev_err(info->dev, "Error in reading the fuel gauge control register\n");
		return ret;
	}
	
	ret = ret * 60;									//conversion unit// 1unit is 1miniute and return nnit should be 1 second
	
	return ret;
}

static int get_time_to_full(struct ricoh618_battery_info *info)
{
	int ret = 0;
	
	ret = get_check_fuel_gauge_reg(info, TT_FULL_H_REG, TT_FULL_L_REG, 0xffff);
	if(ret<0){
		dev_err(info->dev, "Error in reading the fuel gauge control register\n");
		return ret;
	}

	ret = ret * 60;

	return  ret;
}

//battery voltage is get from Fuel gauge
static int measure_vbatt_FG(struct ricoh618_battery_info *info, int *data)
{
	int ret = 0;
	
//	mutex_lock(&info->lock);
	
	ret =  get_check_fuel_gauge_reg(info, VOLTAGE_1_REG, VOLTAGE_2_REG, 0x0fff);	
	if(ret<0){
		dev_err(info->dev, "Error in reading the fuel gauge control register\n");
		mutex_unlock(&info->lock);
		return ret;
	}
		
	*data = ret;
	*data = *data * 5000 / 4095;							//conversion unit 1 Unit is 1.22mv (5000/4095 mv)
	*data = *data * 1000;									//return unit should be 1uV

//	mutex_unlock(&info->lock);

	return ret;
}

#else
//battery voltage is get from ADC
static int measure_vbatt_ADC(struct ricoh618_battery_info *info, int *data)
{
	int	i;
	uint8_t data_l =0, data_h = 0;
	int ret;
	
//	mutex_lock(&info->lock);

	ret = ricoh618_set_bits(info->dev->parent, INTEN_REG, 0x08); 		//ADC interrupt enable
	if(ret <0){
		dev_err(info->dev, "Error in setting the control register bit\n");
		goto err;
	}

	ret = ricoh618_set_bits(info->dev->parent, EN_ADCIR3_REG, 0x01); 	//enable interrupt request of single mode
	if(ret <0){
		dev_err(info->dev, "Error in setting the control register bit\n");
		goto err;
	}

	ret = ricoh618_write(info->dev->parent, ADCCNT3_REG, 0x10);		//single request		
	if(ret <0){
		dev_err(info->dev, "Error in writing the control register\n");
		goto err;
	}

	for(i = 0; i < 5; i++){
		msleep(1);
		dev_info(info->dev, "ADC conversion times: %d\n", i);
		ret = ricoh618_read(info->dev->parent, EN_ADCIR3_REG, &data_h);	//read completed flag of  ADC
		if(ret <0){
			dev_err(info->dev, "Error in reading the control register\n");
			goto err;
		}

		if(data_h && 0x01){
			goto	done;
		}
	}
	
	dev_err(info->dev, "ADC conversion too long!\n");
	goto err;
	
done:
	ret = ricoh618_read(info->dev->parent, VBATDATAH_REG, &data_h);
	if(ret <0){
		dev_err(info->dev, "Error in reading the control register\n");
		goto err;
	}

	ret = ricoh618_read(info->dev->parent, VBATDATAL_REG, &data_l);
	if(ret <0){
		dev_err(info->dev, "Error in reading the control register\n");
		goto err;
	}

	*data = ((data_h & 0xff) << 4) | (data_l & 0x0f);
	*data = *data * 5000 / 4095;								//conversion unit 1 Unit is 1.22mv (5000/4095 mv)
	*data = *data * 1000;									//return unit should be 1uV
	
//	mutex_unlock(&info->lock);

	return 0;
	
err:
	mutex_unlock(&info->lock);
	return -1;
}

#endif

static void ricoh618_external_power_changed(struct power_supply *psy)
{
	struct ricoh618_battery_info *info;

	info = container_of(psy, struct ricoh618_battery_info, battery);
	queue_delayed_work(info->monitor_wqueue,
			   &info->changed_work, HZ / 2);
	return;
}


static int ricoh618_batt_get_prop(struct power_supply *psy,
				enum power_supply_property psp,
				union power_supply_propval *val)
{
	struct ricoh618_battery_info *info = dev_get_drvdata(psy->dev->parent);
	int data = 0;
	int ret = 0;
	
	mutex_lock(&info->lock);

	switch (psp) {
	 //this setting is same as battery driver of 584
	case POWER_SUPPLY_PROP_STATUS:
		ret = get_power_supply_status(info);
		val->intval = ret;
		info->status = ret;
		dev_info(info->dev, "Power Supply Status is %d\n", info->status);
		break;
		
	 //this setting is same as battery driver of 584		
	case POWER_SUPPLY_PROP_PRESENT:
		val->intval = info->present;
		break;
 
	//current voltage is get from fuel gauge
	case POWER_SUPPLY_PROP_VOLTAGE_NOW:
		/* return real vbatt Voltage */
#ifdef	FUEL_GAGE_FUNCTION_ENABLE
		ret = measure_vbatt_FG(info, &data);
#else
		ret = measure_vbatt_ADC(info, &data);
#endif
		val->intval = data;
		info->cur_voltage = data / 1000;		//convert unit uV -> mV
		dev_info(info->dev, "battery voltage is %d mV\n", info->cur_voltage);
		
		break;

#ifdef	FUEL_GAGE_FUNCTION_ENABLE
	//current battery capacity is get from fuel gauge
	case POWER_SUPPLY_PROP_CAPACITY:
		ret = calc_capacity(info);
		val->intval = ret;
		info->capacity = ret;
		dev_info(info->dev, "battery capacity is %d%%\n", info->capacity);
		break;
	
	//current temperature of buttery
	case POWER_SUPPLY_PROP_TEMP:
		ret = get_buttery_temp(info);
		val->intval = ret;
		info->battery_temp = ret/10;
		dev_info(info->dev, "battery temperature is %d degree \n",  info->battery_temp );
		break;
	
	case POWER_SUPPLY_PROP_TIME_TO_EMPTY_NOW:
		ret = get_time_to_empty(info);
		val->intval = ret;
		info->time_to_empty = ret/60;
		dev_info(info->dev, "time of empty buttery is %d minutes \n", info->time_to_empty );
		break;
	
	 case POWER_SUPPLY_PROP_TIME_TO_FULL_NOW:
		ret = get_time_to_full(info);
		val->intval = ret;
		info->time_to_full = ret/60;
		dev_info(info->dev, "time of full buttery is %d minutes \n", info->time_to_full );
		break;
#endif
	 case POWER_SUPPLY_PROP_TECHNOLOGY:
		val->intval = POWER_SUPPLY_TECHNOLOGY_LION;
		ret = 0;
		break;

	case POWER_SUPPLY_PROP_HEALTH:
		val->intval = POWER_SUPPLY_HEALTH_GOOD;
		ret = 0;
		break;
		
	default:
		return -ENODEV;
	}
	
	mutex_unlock(&info->lock);
	
	return ret;
}

static int ricoh618_charger_get_prop(struct power_supply *psy,
				enum power_supply_property psp,
				union power_supply_propval *val)
{
	struct ricoh618_battery_info *info = dev_get_drvdata(psy->dev->parent);
	uint8_t status, supply_state;
	int ret = 0;

	ret = ricoh618_read(info->dev->parent, CHGSTATE_REG, &status);
	if (ret<0){
		dev_err(info->dev, "Error in reading the control register\n");
		return ret;
	}
	supply_state = (status & 0xC0) >> 5;

	switch (psp) {
	case POWER_SUPPLY_PROP_ONLINE:
		if (psy->type == POWER_SUPPLY_TYPE_MAINS) {
			val->intval = supply_state & (1 << SUPPLY_STATE_ADP);
		} else if (psy->type == POWER_SUPPLY_TYPE_USB) {
			val->intval = supply_state & (1 << SUPPLY_STATE_USB);
		} else
			val->intval = 0;
		break;
	default:
		return -EINVAL;
	}

	return 0;
}

static irqreturn_t ricoh618_ac_irq(int irq, void *dev_id)
{
	struct ricoh618_battery_info *info = dev_id;

//	power_supply_changed(&info->ac);

	return IRQ_HANDLED;
}

static irqreturn_t ricoh618_usb_irq(int irq, void *dev_id)
{
	struct ricoh618_battery_info *info = dev_id;

	dev_dbg(info->dev,"----> %s \n", __func__);

	power_supply_changed(&info->usb);

	return IRQ_HANDLED;
}

static enum power_supply_property ricoh618_batt_props[] = {
	POWER_SUPPLY_PROP_STATUS,
	POWER_SUPPLY_PROP_PRESENT,
	POWER_SUPPLY_PROP_VOLTAGE_NOW,

#ifdef	FUEL_GAGE_FUNCTION_ENABLE
	POWER_SUPPLY_PROP_CAPACITY,
	POWER_SUPPLY_PROP_TEMP,
	POWER_SUPPLY_PROP_TIME_TO_EMPTY_NOW,
	POWER_SUPPLY_PROP_TIME_TO_FULL_NOW,
#endif
	POWER_SUPPLY_PROP_TECHNOLOGY,
	POWER_SUPPLY_PROP_HEALTH,
};

static enum power_supply_property ricoh618_charger_props[] = {
	POWER_SUPPLY_PROP_ONLINE,
};

static char *supply_list[] = {
	"battery",
};

static __devinit int ricoh618_battery_probe(struct platform_device *pdev)
{
	struct ricoh618 *iodev = dev_get_drvdata(pdev->dev.parent);
	struct pmu_platform_data *pmu_pdata = dev_get_platdata(iodev->dev);
	struct ricoh618_battery_info *info;
	struct ricoh618_battery_platform_data *pdata;

	int ret = 0;

	printk("-------->  %s: \n",__func__);

	info = kzalloc(sizeof(struct ricoh618_battery_info), GFP_KERNEL);
	if (!info)
		return -ENOMEM;

	info->dev = &pdev->dev;
	info->status = POWER_SUPPLY_STATUS_CHARGING;
	pdata = (struct ricoh618_battery_platform_data *)pmu_pdata->bat_private;
	info->monitor_time = pdata->monitor_time * HZ;
	info->alarm_vol_mv = pdata->alarm_vol_mv;
	info->adc_vdd_mv = ADC_VDD_MV;	//2800;
	info->min_voltage = MIN_VOLTAGE;	//3100;
	info->max_voltage = MAX_VOLTAGE;	//4200;

	mutex_init(&info->lock);
	platform_set_drvdata(pdev, info);
	if (!((&pdev->dev)->p)) {
		printk("-----> platform_set_drvdata error  \n");
		goto out;
	}

	info->battery.name = "battery";
	info->battery.type = POWER_SUPPLY_TYPE_BATTERY;
	info->battery.properties = ricoh618_batt_props;
	info->battery.num_properties = ARRAY_SIZE(ricoh618_batt_props);
	info->battery.get_property = ricoh618_batt_get_prop;
	info->battery.set_property = NULL;
	info->battery.external_power_changed = ricoh618_external_power_changed;

	ret = ricoh618_init_battery(info);
	if (ret){
		goto out;
	}


	ret = power_supply_register(&pdev->dev, &info->battery);
	if (ret){
		info->battery.dev->parent = &pdev->dev;
	}

#define DEF_CHARGER(PSY, NAME, TYPE)					\
	info->PSY.name = NAME;						\
	info->PSY.type = TYPE;						\
	info->PSY.supplied_to = supply_list;				\
	info->PSY.num_supplicants = ARRAY_SIZE(supply_list);		\
	info->PSY.properties = ricoh618_charger_props;			\
	info->PSY.num_properties = ARRAY_SIZE(ricoh618_charger_props);	\
	info->PSY.get_property = ricoh618_charger_get_prop

	DEF_CHARGER(usb, "usb", POWER_SUPPLY_TYPE_USB);
	DEF_CHARGER(ac, "ac", POWER_SUPPLY_TYPE_MAINS);
#undef DEF_POWER
	power_supply_register(&pdev->dev, &info->usb);
	power_supply_register(&pdev->dev, &info->ac);

	info->irq_usb = RICOH618_IRQ_FVUSBDETSINT;
	info->irq_ac = RICOH618_IRQ_FVADPDETSINT;

	ret = request_threaded_irq(info->irq_ac, NULL, ricoh618_ac_irq,
			  IRQF_DISABLED, "ricoh618_ac_irq", info);
	if (ret) {
		dev_err(info->dev, "request ricoh618_ac_irq fail with error num = %d \n", ret);
		goto out;
	}
	ret = request_threaded_irq(info->irq_usb, NULL, ricoh618_usb_irq,
			  IRQF_DISABLED, "ricoh618_usb_irq", info);
	if (ret) {
		dev_err(info->dev, "request ricoh618_ac_irq fail with error num = %d \n", ret);
		goto out;
	}


	info->monitor_wqueue = create_singlethread_workqueue("ricoh618_battery_monitor");
	INIT_DELAYED_WORK_DEFERRABLE(&info->monitor_work, ricoh618_battery_work);
	INIT_DELAYED_WORK(&info->changed_work, ricoh618_changed_work);
	queue_delayed_work(info->monitor_wqueue, &info->monitor_work,
			   info->monitor_time);

	return 0;

out:
	kfree(info);
	printk("-------> ;alkdjflksjdfl;akjflasdjkf \n");
	return ret;
}

static int __devexit ricoh618_battery_remove(struct platform_device *pdev)
{
	struct ricoh618_battery_info *info = platform_get_drvdata(pdev);

	cancel_delayed_work(&info->monitor_work);
	cancel_delayed_work(&info->changed_work);
	flush_workqueue(info->monitor_wqueue);
	power_supply_unregister(&info->battery);
	kfree(info);
	platform_set_drvdata(pdev, NULL);
	return 0;
}

#ifdef CONFIG_PM
static int ricoh618_battery_suspend(struct device *dev)
{
	struct ricoh618_battery_info *info = dev_get_drvdata(dev);

	cancel_delayed_work_sync(&info->monitor_work);

	return 0;
}

static int ricoh618_battery_resume(struct device *dev)
{
	struct ricoh618_battery_info *info = dev_get_drvdata(dev);

	queue_delayed_work(info->monitor_wqueue, &info->monitor_work,
			   info->monitor_time);
	return 0;
}

static struct dev_pm_ops ricoh618_battery_pm_ops = {
	.suspend	= ricoh618_battery_suspend,
	.resume		= ricoh618_battery_resume,
};
#endif

static struct platform_driver ricoh618_battery_driver = {
	.driver	= {
				.name	= "ricoh618-battery",
				.owner	= THIS_MODULE,
#ifdef CONFIG_PM
				.pm	= &ricoh618_battery_pm_ops,
#endif
	},
	.probe	= ricoh618_battery_probe,
	.remove	= __devexit_p(ricoh618_battery_remove),
};

static int __init ricoh618_battery_init(void)
{
	return platform_driver_register(&ricoh618_battery_driver);
}
module_init(ricoh618_battery_init);

static void __exit ricoh618_battery_exit(void)
{
	platform_driver_unregister(&ricoh618_battery_driver);
}
module_exit(ricoh618_battery_exit);

MODULE_DESCRIPTION("RICOH618 Battery driver");
MODULE_ALIAS("platform:ricoh618-battery");
MODULE_LICENSE("GPL");
