#ifndef __EVENT_H__
#define __EVENT_H__
#include <base_type.h>
#include <config.h>
#include <App_timer.h>
#include "wy_gpio.h"
#define MODULE_EVENT				"EVENT"

#define TIMER_COMMENT_TICKS          	APP_TIMER_TICKS(200, APP_TIMER_PRESCALER)
#define TIMER_RTC_TICKS          		APP_TIMER_TICKS(3000, APP_TIMER_PRESCALER)
typedef struct timer_event{
	H_U32 timer_id;
	H_U32 timer_timeout;
	app_timer_mode_t mode;
	app_timer_timeout_handler_t handler;
}timer_event_t;


struct event_manage{
	timer_event_t timer_common;
	timer_event_t timer_rtc;
#ifdef MODULE_TOUCH_KEYBOARD_ENABLE
#if (TOUCH_KEYBOARD_TYPE == TOUCH_KEYBOARD_TTY6952)
	struct gpio_list tty6952;
#endif
#endif
};

H_U32 _EventOpen(void);

#endif
