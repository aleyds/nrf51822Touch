
#ifndef __SYSTEM_H__
#define __SYSTEM_H__
#include <base_type.h>
#include "pstorage.h"

#define SYSTEM_MODULE         "SYSTEM"

#define LOCK_MODE_FAMILY		(0x0)   /*家庭模式*/
#define LOCK_MODE_OFFICE		(0x1)	/*办公室模式*/

//#define BUSY                    (0X1)

#define LOCK_STATE_DETECTION	(LOCK_STATE_PORT)
#define DOOR_STATE_DETECTION	(DOOR_STATE_PORT)
//status of door and lock
#define UNLOCKED				(0x1)
#define LOCKED					(0x0)	
#define CLOSED                  (0x0)
#define OPENED   				(0x1)

//time calculate for Timer_common,when Timer_common timeout is 200ms
#define ONE_WIRE_INTERVAL_ON_TIMER_COMMON         (4UL)     //800ms
#define BATTERY_MEASURE_ON_TIMER_COMMON           (36000UL) //2hours
#define CONNECTION_LIMIT_ON_TIMER_COMMON          (1800UL)  //3mins
#define AUTO_LOCK_INTERVAL_ON_TIMER_COMMON        (30UL)    //6seconds
#define ADD_LOCK_PROMPT_DELAY_ON_TIMER_COMMON     (15UL)    //3seconds
#define ALARM_TIME_ON_TIMER_COMMON                (300UL)   //1min
#define SKEY_MODE_SWITCH_ON_TIMER_COMMON          (150)     //30s
#define ADMIN_PWD_VERIFY_ON_TIMER_COMMON          (150)     //30s
#define FINGER_MODE_TIMER_COMMON					(300)   //1min
#define TOUCH_RESTART_COMMON					(600)

#define SYSTEM_MAGIC_LEN				(4)
#define SYSTEM_MAGIC_DATA				(0x7379736d)
#define SYSTEM_MAGIC_TURN				(0x6d737973)

typedef struct sys_voice{
	H_BOOL voice_played;
	H_BOOL closed;
	H_BOOL close_fail;
}sys_voice_t;

struct system_data{
	H_U32 magic;
	H_U8 lock_state;	//0 unlocked ;1 locked
	H_U8 door_state;	//0 close ;1 open
	H_U8 mode_switch;   //0 family mode ; 1 office mode
	H_U8 lock_state_prev; //for IO change detection 
	H_U8 door_state_prev;
	H_U8 add_lock_prompt; 
	H_U8 alarm_detect_state;
	H_U8 reboot;
	sys_voice_t voice;
};

struct exra_data{
	H_U32 admin_pwd_verify;
};

//global varibles for timer common
typedef struct system_intervals{
	H_U32 _one_wire_interval;         //done
	H_U32 _battery_measure_interval;  //done
	H_U32 _connection_limit;          //to-do
	H_U32 _auto_lock_interval;        //done
	H_U32 _add_lock_prompt_interval;  //done
	H_U32 _alarm_on_interval;         //done       
	H_U32 _skey_add_mode_interval;    //done
	H_U32 _admin_pwd_verify_interval; //done
	H_U32 _factory_reset_interval;    //to-do
	H_U32 _touch_key_led_interval;
	H_U32 _finger_mode_interval;
	H_U32 _touch_restart;
}system_intervals_t;

//global varibles for system condition
//typedef struct system_conditions{
//	
//}sysem_conditions_t;



struct system_manage{
	pstorage_handle_t pg_handler;
	struct system_data data;
	struct exra_data edata;
	system_intervals_t _system_timer;
};

typedef struct system_data*(*fun_system_data_get)(void);



typedef struct system_device{
	H_U32 type;
	uint32_function _open;
	//uint32_function _voice_played_status_get;
	//uint32_function_uint32 _voice_played_status_set;
	uint32_function _mode_get;
	uint32_function_uint32 _mode_set;
	uint32_function _lock_state;
	uint32_function _door_state;
	uint32_function _system_reset;
	uint32_function _system_reboot;
	uint32_function_uint32 _verify_set;
	uint32_function _verify_get;
	uint32_function _system_timer_common_handle;
	function_void _sys_data_save;
	function_void _touch_led_clear;
	function_void _touch_led_set;
	function_void _finger_mode_reset;
	fun_system_data_get _sys_get;
}system_device_t;


system_device_t* wy_system_op(void);



#endif

