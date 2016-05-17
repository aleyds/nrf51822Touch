
#include <boards.h>
#include <nrf_gpio.h>
#include "wy_control_common.h"
#include "wy_alarm.h"

#ifdef MODULE_ALARM_ENABLE

static struct alarm_manage g_alarm_manage;

static struct alarm_manage* __alarm_manage_get(void)
{
	return (struct alarm_manage *)&g_alarm_manage;
}

static H_U32 __alarm_open(void)
{
	nrf_gpio_cfg_output(ALARM_ENABLE_PORT);
	nrf_gpio_pin_clear(ALARM_ENABLE_PORT);
	wy_memset(&g_alarm_manage, 0, sizeof(struct alarm_manage));
	//test for big buzzer
	return H_SUCCESS;
}

static H_U32 __alarm_port_onoff(H_BOOL gate)
{
	struct alarm_manage* manage = __alarm_manage_get();
	if(gate)
	{
		nrf_gpio_pin_set(ALARM_ENABLE_PORT);
		#ifdef MODULE_BIG_BUZZER_ENABLE
			nrf_gpio_pin_set(ALARM_BIG_BUZZER);
		#endif
		manage->alarm_port_status = ALARM_ON;
	}
	else
	{
		if(ALARM_ON == manage->alarm_port_status)
		{
			if(manage->alarm_port_status == ALARM_PORT_CLOSE)
			{
				nrf_gpio_pin_clear(ALARM_ENABLE_PORT);
				manage->alarm_port_status = ALARM_OFF;
				manage->alarm_port_status = ALARM_PORT_NCLOSE;
			}
			else
			{
				manage->alarm_port_status = ALARM_PORT_CLOSE;
			}
			
		}
		
	}
	return 0;
}

static H_U32 __alarm_onoff(H_BOOL gate, H_BOOL feedback)
{
	struct alarm_manage* manage = __alarm_manage_get();
	H_U8 alarm = ALARM_OFF;
	if((gate) && (ALARM_OFF == manage->alarm_status))
	{
		nrf_gpio_pin_set(ALARM_ENABLE_PORT);
		manage->alarm_status = ALARM_ON;
		alarm =  ALARM_ON;
	}
	else if((!gate) && (ALARM_ON == manage->alarm_status))
	{
		nrf_gpio_pin_clear(ALARM_ENABLE_PORT);
		manage->alarm_status = ALARM_OFF;
		alarm =  ALARM_OFF;
	}
	if(feedback)
	{
		wy_near_control_op()->_feedback(CTL_BACK_ALARM_STATUS,H_NULL,0);
	}
	
	return alarm;
}

static H_U8 __alarm_get_status(void)
{
	struct alarm_manage* manage = __alarm_manage_get();
	return manage->alarm_status;
}

alarm_device_t* wy_alarm_op(void)
{
	static alarm_device_t ___alarm_op = {
			._open = __alarm_open,
			._onoff = __alarm_onoff,
			._port_onoff = __alarm_port_onoff,
			._status_get = __alarm_get_status,
	};
	return &___alarm_op;
}
#endif

