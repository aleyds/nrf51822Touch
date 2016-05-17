
#include <wy_tools.h>
#include <wy_uart.h>
#include "wy_alarm.h"
#include "wy_system.h"
#ifdef MODULE_BATTERY_ENABLE
#include "wy_battery.h"
#endif
#include <wy_control_common.h>
#include <wy_control_core.h>

#ifdef MODULE_TOUCH_KEYBOARD_ENABLE
#include "wy_touch.h"
#endif
/**************************************/
#define SYNC_TIME_LOG_WRITE_DEVIATION			(10)  //10s Deviation


//static H_U8 modify_key[8];

static H_S32 __near_core_get_info(struct ctl_command *cmd, struct ctl_feedback *back)
{
	CHECK_PARAMETER(cmd,H_NULL,H_FAULTER);
	CHECK_PARAMETER(back,H_NULL,H_FAULTER);
	//Step2:set feedback
	back->type = cmd->back_type;
	back->counter = cmd->counter + 1;
	back->control_type = cmd->control_type;
	back->back_flag = FEEDBACK_STATE;

	back->data.info.battery_level = wy_battery_op()->_get_level_direct();
	back->data.info.lock_state = wy_system_op()->_lock_state();
	back->data.info.door_state = wy_system_op()->_door_state();
#ifdef MODULE_ALARM_ENABLE
	back->data.info.alarm_state = wy_alarm_op()->_status_get();
#endif
	back->data.info.mode_switch = wy_system_op()->_mode_get();
	return H_SUCCESS;
}

H_S32 wy_near_get_stauts(struct ctl_command *cmd, struct ctl_feedback *back)
{
	CHECK_PARAMETER(cmd,H_NULL,H_FAULTER);
	CHECK_PARAMETER(back,H_NULL,H_FAULTER);
	__near_core_get_info(cmd, back);
	return H_SUCCESS;
}



#ifdef MODULE_ALARM_ENABLE
H_S32 wy_near_alarm(struct ctl_command *cmd, struct ctl_feedback *back)
{
	CHECK_PARAMETER(cmd, H_NULL, H_FAULTER);
	CHECK_PARAMETER(back, H_NULL, H_FAULTER);
	H_U8 user_pin;
    user_pin = wy_near_control_op()->_get_pin();
	if(cmd->type == CTL_SOUND_ALARM)
	{
		wy_alarm_op()->_onoff(H_TRUE,H_TRUE); /*¿ªÆô·äÃùÆ÷*/
	}
	else if(cmd->type == CTL_DISALARM)
	{
		wy_alarm_op()->_onoff(H_FALSE,H_TRUE);/*¹Ø±Õ·äÃùÆ÷*/
	}

	__near_core_get_info(cmd, back);
	return H_SUCCESS;
}
#endif

#ifdef MODULE_TOUCH_KEYBOARD_ENABLE
H_S32 wy_touch_key_modify(struct ctl_command *cmd, struct ctl_feedback *back)
{

}
#endif



#ifdef MODULE_DFU_ENABLE
H_S32 m2mkey_dfu_enter(struct ctl_command *cmd, struct ctl_feedback *back)
{
	back->counter = cmd->counter + 1;
	back->type = cmd->back_type;
	back->control_type = cmd->control_type;
	wy_tools_op()->_dfu_mode();
}
#endif


