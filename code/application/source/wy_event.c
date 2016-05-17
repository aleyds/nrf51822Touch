#include "list.h"
#include "wy_gpio.h"
#include "wy_timers.h"
#include "wy_uart.h"
#include <wy_storage.h>
#include <softdevice_handler.h>
#include "wy_battery.h"
#include "wy_event.h"

#include "wy_system.h"
#include "wy_control_common.h"
#include "wy_alarm.h"
#include "wy_tools.h"
#ifdef MODULE_TOUCH_KEYBOARD_ENABLE
#include "wy_touch.h"
#endif
/*****************Timer Event*******************/
static struct event_manage g_event_manage;

static struct event_manage* __event_manage_get(void)
{
	return (struct event_manage *)&g_event_manage;
}

//should do all the things done in TIMER0 HANDLE at BL600 CODE
static void __timer_common(void * p_context)
{
    UNUSED_PARAMETER(p_context);
	wy_system_op()->_system_timer_common_handle();
	
	
}

#ifdef MODULE_TOUCH_KEYBOARD_ENABLE
#if ((TOUCH_KEYBOARD_TYPE == TOUCH_KEYBOARD_TTY6952) || (TOUCH_KEYBOARD_TYPE == TOUCH_KEYBOARD_BS8112A3))
static void __gpio_touch_callback(H_U32 lth, H_U32 htl)
{
	WYPrintf(MODULE_EVENT ,LEVEL_DEBUG,"__gpio_touch_callback lth:%x htl:%x",lth, htl);
	
	if(lth > 0)
	{
		_TouchRead();
	}
	else if(htl > 0)
	{
		
	}
}
#endif
#endif


static void __event_init(void)
{
	struct event_manage* manage = __event_manage_get();
/**************************Timer Event******************************************/
	manage->timer_common.mode = APP_TIMER_MODE_REPEATED;
	manage->timer_common.timer_timeout = TIMER_COMMENT_TICKS;
	manage->timer_common.handler = __timer_common;
	
	
/***************************GPIO Event********************************************/
#ifdef MODULE_TOUCH_KEYBOARD_ENABLE
#if ((TOUCH_KEYBOARD_TYPE == TOUCH_KEYBOARD_TTY6952) || (TOUCH_KEYBOARD_TYPE == TOUCH_KEYBOARD_BS8112A3))
	nrf_gpio_cfg_sense_input(TOUCH_TTY6952_IRQ, GPIO_PIN_CNF_PULL_Disabled, GPIO_PIN_CNF_SENSE_Low);
	manage->tty6952.user_info.pins_high_to_low_mask = (1 << TOUCH_TTY6952_IRQ);
	manage->tty6952.user_info.pins_low_to_high_mask = (1 << TOUCH_TTY6952_IRQ);
	manage->tty6952.user_info.pin_mask = manage->tty6952.user_info.pins_high_to_low_mask | manage->tty6952.user_info.pins_low_to_high_mask;
	manage->tty6952.user_info.pin_number = TOUCH_TTY6952_IRQ;
	manage->tty6952.user_info._callback = __gpio_touch_callback;
#endif
#endif
	
}

static void __event_start(void)
{
	struct event_manage* manage = __event_manage_get();

	//通用定时器定义申请
	_TimerCreate(&(manage->timer_common.timer_id) , manage->timer_common.mode, manage->timer_common.handler);
	_TimerStart(manage->timer_common.timer_id,manage->timer_common.timer_timeout, H_NULL);

#ifdef MODULE_TOUCH_KEYBOARD_ENABLE
#if ((TOUCH_KEYBOARD_TYPE == TOUCH_KEYBOARD_TTY6952) || (TOUCH_KEYBOARD_TYPE == TOUCH_KEYBOARD_BS8112A3))
	_GpioJoin(&(manage->tty6952));
	_GpioOnOff(manage->tty6952.user_info.pin_number, H_TRUE);
#endif
#endif

	
}

H_U32 _EventOpen(void)
{
	WYPrintf(MODULE_EVENT ,LEVEL_DEBUG,"aleyds : event open ");
	__event_init();
	__event_start();
	return H_SUCCESS;
}

