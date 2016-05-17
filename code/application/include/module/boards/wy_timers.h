/**********************************************************************
***********************************************************************/
#ifndef __TIMERS_H__
#define __TIMERS_H__
#include <base_type.h>
#include <app_error.h>
#include <app_timer.h>

#define APP_TIMER_PRESCALER             0                                           /**< Value of the RTC1 PRESCALER register. */
#define APP_TIMER_MAX_TIMERS            5                                           /**< Maximum number of simultaneously created timers. */
#define APP_TIMER_OP_QUEUE_SIZE         5                                           /**< Size of timer operation queues. */


void _TimerOpen(void);
H_U32 _TimerCreate(app_timer_id_t *p_timer_id,app_timer_mode_t mode,
	                     app_timer_timeout_handler_t timeout_handler);
	                     
H_U32 _TimerStart(app_timer_id_t timer_id, H_U32 timeout_ticks, void * p_context);
H_U32 _TimerStop(app_timer_id_t timer_id);

#endif

