/**********************************************************************
***********************************************************************/
#include <wy_timers.h>

void _TimerOpen(void)
{
   APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_MAX_TIMERS, APP_TIMER_OP_QUEUE_SIZE, false);
}

H_U32 _TimerCreate(app_timer_id_t *p_timer_id,app_timer_mode_t mode,
	                     app_timer_timeout_handler_t timeout_handler)
{
	
	H_U32 err_code = 0;
	err_code = app_timer_create(p_timer_id,mode,timeout_handler);
	//APP_ERROR_CHECK(err_code);
	return H_SUCCESS;
}

H_U32 _TimerStart(app_timer_id_t timer_id, H_U32 timeout_ticks, void * p_context)
{
	H_U32 err_code = 0;

	// Start application timers
	err_code = app_timer_start(timer_id, timeout_ticks, p_context);
	//APP_ERROR_CHECK(err_code);
	return H_SUCCESS;
}

H_U32 _TimerStop(app_timer_id_t timer_id)
{
	H_U32 err_code = 0;

	// Stop application timers
	err_code = app_timer_stop(timer_id);
	//APP_ERROR_CHECK(err_code);
	return H_SUCCESS;
}

