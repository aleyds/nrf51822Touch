/* Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */
#include <base_type.h>
#include <nrf_ecb.h>
#include <softdevice_handler.h>
#include <lib_i2c.h>
#include "wy_ble_common.h"
#include "wy_storage.h"
#include "wy_light.h"
#include "wy_timers.h"
#include "wy_tools.h"
#include "wy_uart.h"

#include "wy_control_common.h"

#include "wy_battery.h"

#include "wy_gpio.h"
#ifdef MODULE_ALARM_ENABLE
#include "wy_alarm.h"
#endif
#include "wy_event.h"


#include "wy_system.h"
#ifdef MODULE_TOUCH_KEYBOARD_ENABLE
#include "wy_touch.h"
#endif

//使能flash存取数据时为1byte.
#pragma pack (1)

/**************************extern********************/
#define MODULE_MAIN							"MAIN"


static H_S32 __system_init()
{
    WYPrintf(MODULE_MAIN,LEVEL_DEBUG,"System INIT");
	/**********ECB*************/
	nrf_ecb_init();
	/***********GPIO Event Open*********/
	_GpioOpen();

#ifdef MODULE_ALARM_ENABLE
	/***********Alarm Open**************/
	wy_alarm_op()->_open();
	WYPrintf(MODULE_MAIN,LEVEL_DEBUG,"Alarm Open");
#endif
	
	wy_system_op()->_open();
#ifdef MODULE_BATTERY_ENABLE
	/********Battery********************/
	wy_battery_op()->_open();
#endif

#ifdef MODULE_TOUCH_KEYBOARD_ENABLE
	_TouchOpen();
#endif
	
	return H_SUCCESS;
}

static H_S32 __hardware_init()
{


#ifdef MODULE_LED_ENABLE
    /*********LED Light**********/
	_LightOpen();
#endif

/************Uart*****************/
#if defined MODULE_UART_ENABLE
		_UartOpen();
		wy_tools_op()->_delay_us(50);
#else
		_UartClose();
#endif
	WYPrintf(MODULE_MAIN,LEVEL_DEBUG,"Hardware initialize  ");
	/**********Storage****************/
	_StorageOpen();
	wy_tools_op()->_delay_us(50);
	
	/**********TIMER***********/
	_TimerOpen();


	/************Bluetooth************/
	wy_bluetooth_op()->_open();

	/**bluetooth adv service**/
	wy_bluetooth_op()->_adv._open();
	wy_bluetooth_op()->_adv._start();

	wy_near_control_op()->_open();

	
	wy_bluetooth_op()->_soft_callback();
	wy_bluetooth_op()->_connect_config();


#ifdef LIB_I2C_ENABLE
	if(lib_i2c_open())
	{
	}
#endif

	return 0;
}


static void _GpioInit(void)
{
	for(H_U32 i = 0; i < 32; i++)
	{
		nrf_gpio_cfg_input_null(i);
	}
}

static void _SystemSleep(void)
{
	while(H_TRUE)
	{
		sd_app_evt_wait();
	}
}

/**@brief  Application main function.
 */
int main(void)
{  

    //Disable All GPIO 
    _GpioInit();
	
	__hardware_init();
	__system_init();

	_EventOpen();

	_SystemSleep();//系统在这里睡眠等待中断处理
			
}


