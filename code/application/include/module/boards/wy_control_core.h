#ifndef __NEAR_CTL_SORE_H__
#define __NEAR_CTL_SORE_H__
#include <base_type.h>
#define MODULE_NEAR_CORE "NEAR_CORE"


H_S32 wy_near_get_stauts(struct ctl_command *cmd, struct ctl_feedback *back);
H_S32 wy_near_updata(struct ctl_feedback *back);

#ifdef MODULE_ALARM_ENABLE
H_S32 wy_near_alarm(struct ctl_command *cmd, struct ctl_feedback *back);
#endif
H_S32 wy_near_mode_switch(struct ctl_command *cmd, struct ctl_feedback *back);


#ifdef MODULE_TOUCH_KEYBOARD_ENABLE
H_S32 wy_touch_key_modify(struct ctl_command *cmd, struct ctl_feedback *back);
#endif


#ifdef MODULE_DFU_ENABLE
H_S32 m2mkey_dfu_enter(struct ctl_command *cmd, struct ctl_feedback *back);
#endif


#endif
