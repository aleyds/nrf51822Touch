#ifndef __ALARM_H__
#define __ALARM_H__
#include "base_type.h"

#define MODULE_DEBUG_ALARM "ALARM"

#define ALARM_ENABLE_PORT			(ALARM_PORT)
#define ALARM_BIG_BUZZER			(BIG_ALARM_BUZZER)
#define ALARM_ON					(0x1)
#define ALARM_OFF					(0x0)
#define ALARM_PORT_CLOSE			(0x1)
#define ALARM_PORT_NCLOSE			(0x0)

typedef H_U32 (*alarm_onoff)(H_BOOL,H_BOOL);
typedef H_U32 (*alarm_port_onoff)(H_BOOL);
typedef H_U8 (*alarm_status_get)(void);


typedef struct alarm_device{
	H_U32 type;
	uint32_function _open;
	alarm_onoff _onoff;
	alarm_port_onoff _port_onoff;
	alarm_status_get _status_get;
}alarm_device_t;

struct alarm_manage{
	H_U8 alarm_status;	
	H_U8 alarm_port_status;
	H_U8 alarm_port_close;
};

alarm_device_t* wy_alarm_op(void);


#endif

