
#ifndef __NEAR_CONTROL_H__
#define __NEAR_CONTROL_H__
#include <base_type.h>
#include <list.h>
#include "wy_control_common.h"
#include <ble_nus.h>

#define MODULE_NEAR_CONTROL			"NEAR_CONTROL"

#define CTL_ACCESS_USER (0x100)
#define CTL_ACCESS_ADMIN (CTL_ACCESS_USER<<1)
#define CTL_ACCESS_ALL   (CTL_ACCESS_USER | CTL_ACCESS_ADMIN)

#define CTL_HAND_DATA_LEN		(4)
#define CTL_COMMAND_DATA_LEN	(16)
#define CTL_FEEDBACK_DATA_LEN	(16)
#define CTL_RANDOM_DATA_LEN		(16)
#define CTL_MAC_DATA_LEN		(4)



#define CTL_GET_LOCK_INFO		(0)
#define CTL_LOCKED				(1)
#define CTL_UNLOCKED			(2)
#define CTL_OFFICE_MODE			(3)
#define CTL_FAMILY_MODE			(4)
#define CTL_SOUND_ALARM			(5)
#define CTL_DISALARM			(6)
#define CTL_ADD_SKEY			(7)
#define CTL_RUN_SKEY			(8)
#define CTL_READ_SKEYID			(9)
#define CTL_DELETE_SKEY			(10)
#define CTL_DELETE_ALL_SKEY		(11)
#define CTL_READ_LOG			(12)
#define CTL_ADD_LTK				(13)
#define CTL_DELETE_LTK			(14)
#define CTL_TIME_SYNC			(15)
#define CTL_ADMIN_PWD_VERIFY	(16)
#define CTL_ADMIN_PWD_MODIFY	(17)
#define CTL_DELETE_ALL_LTK		(18)
#define CTL_READ_SKEY			(19)
#define CTL_WIFI_SSID_SET       (21)
#define CTL_WIFI_PWD_SET        (22)
#define CTL_WIFI_TYPE_SET       (23) 
#define CTL_WIFI_AUTH_CODE_SET  (24) 
#define CTL_WIFI_CONNECT        (25) //to-do:connect AP, connect server, send auth code, wait for feedback from server
#define CTL_FINGER_CONFIG		(30)
#define CTL_FINGET_DELETE		(31)
#define CTL_FINGER_RESET		(32)
#define CTL_TOUCHKEY_MODIFY     (35)
#define CTL_ENTER_DFU			(40)


#define CTL_BACK_LOCK_INFO		(0)
#define CTL_BACK_SWITCH_CHANGE	(1)
#define CTL_BACK_LOCK_STATUS	(2)
#define CTL_BACK_BATTERY		(3)
#define CTL_BACK_ALARM_STATUS	(4)
#define CTL_BACK_MOTOR_STATUS	(5)
#define CTL_BACK_SKEY_MODE		(6)
#define CTL_BACK_SKEY_ADD		(7)
#define CTL_BACK_READ_LOG		(8)
#define CTL_BACK_DELETE_SKEY	(10)
#define CTL_BACK_SESSION_START	(13)
#define CTL_BACK_MKEY_LTK		(14)
#define CTL_BACK_ADD_LTK		(15)
#define CTL_BACK_DELETE_LTK		(16)
#define CTL_BACK_VERIFY_PWD		(17)
#define CTL_BACK_MODIFY_PWD		(18)
#define CTL_BACK_POWER_RESTART	(20)
#define CTL_BACK_WIFI_SSID_SET  (22)
#define CTL_BACK_WIFI_PWD_SET   (23)
#define CTL_BACK_WIFI_TYPE_SET  (24)
#define CTL_BACK_WIFI_AUTH_CODE (25)
#define CTL_BACK_WIFI_CON_AP    (26)  //actively notify the APP, after the connect cmd sent ,after connection setup with AP
#define CTL_BACK_SERVER_VERIFY  (27) //actively notify the APP, no command sent from APP before
#define CTL_BACK_FINGER_CONFIG	(30)
#define CTL_BACK_FINGER_DELETE	(31)
#define CTL_BACK_FINGER_RESET	(32)
#define CTL_BACK_TOUCHKEY_MODIFY (35)
#define CTL_BACK_ENTER_DFU		 (40)

#define FEEDBACK_STATE			H_TRUE
#define FEEDBACK_OTHER			H_FALSE

#define CTL_SESSION_TYPE		(12)
#define CTL_SESSION_BACK		"ready"

struct feedback_data_info{
	H_U8 battery_level;
	H_U8 lock_state:1;
	H_U8 door_state:1;
	H_U8 alarm_state:1;
	H_U8 mode_switch:1;
	H_U8 skey_mode:1;
	H_U8 motor_state:1;
	H_U8 time_sync:1;
	H_U8 reserve:1;
};

struct feedback_data_data{
	H_U8 data[CTL_FEEDBACK_DATA_LEN - CTL_HAND_DATA_LEN];
};

enum control_type{
	_TYPE_BLE_CONTROL = 0x0,
	_TYPE_REMOTE_CONTROL,
	_TYPE_REMOTE_PUSH,
	_TYPE_CONTROL_MAX,
};

struct ctl_feedback{
	H_U16 counter;
	H_U16 type;
	H_U16 control_type;
	H_BOOL back_flag;
	union {
		struct feedback_data_info info;
		struct feedback_data_data extra_data;
	}data;
};

 struct ctl_command{
	H_U16 counter;
	H_U16 type;
	H_U16 back_type;
	H_U16 control_type;/*0 近程   1:远程*/
	H_U32 access;/*访问权限*/
	H_U8 data[CTL_FEEDBACK_DATA_LEN - CTL_HAND_DATA_LEN];
};

typedef H_S32 (*ble_control)(struct ctl_command *, struct ctl_feedback *);
typedef H_S32 (*ble_feedback)(struct ctl_feedback *);
#define WIFI_BLE_SSID_LEN			(32)
#define WIFI_BLE_PWD_LEN			(32)
#define WIFI_BLE_VERIFY_LEN			(8)

typedef struct wifi_ssid{
	H_U8 ssid_len;
	H_S8 ssid[WIFI_BLE_SSID_LEN];
}wifi_ssid_t;

typedef struct wifi_password{
	H_U8 pwd_len;
	H_S8 pwd[WIFI_BLE_PWD_LEN];
}wifi_password_t;

struct wifi_verify{

	H_U8 verify_key[WIFI_BLE_VERIFY_LEN];
};

typedef struct near_control_manage{
	ble_nus_t handler;
	H_U8 user_pin_num;
	H_U32 access;
	H_U32 ctl_counter;
	H_U8 random[CTL_RANDOM_DATA_LEN];
	H_U8 msg_sesskey[CTL_COMMAND_DATA_LEN];
	H_U8 mac_sesskey[CTL_COMMAND_DATA_LEN];
	H_U8 *init_vector;
	H_U8 *macsalt;
	H_U8 *msgsalt;
	H_U8 *addkeysalt;
	wifi_ssid_t wifi_ssid;
	wifi_password_t wifi_password;
	struct wifi_verify wifi_verify;
	H_U8 wifi_type;
}near_control_manage_t;

struct bluetooth_command{
	struct list_head entries;
	H_U32 cmd_type;
	H_U32 feedback_type;
	H_U32 access;
	ble_control _cmd_cb;
	ble_feedback _bk_cb;
};

typedef void (*near_open)(void );
typedef H_S32 (*send_nonce)( H_U8 *data, H_U32 len);
typedef void (*send_random)(void);
typedef void (*near_event)(ble_evt_t * p_ble_evt);
typedef H_S32 (*feedbakk)(H_U32 , const H_U8 *,H_U32);
typedef H_S32 (*near_control_nonce_data)(H_U8 *);
typedef H_U32 (*is_connected)(void);
typedef H_U32 (*wifi_ssid_store)(H_U8*,H_U8,H_U8);
typedef H_U32 (*wifi_pwd_store)(H_U8*,H_U8,H_U8);
typedef H_U32 (*wifi_type_store)(H_U8);
typedef void (*set_pin_num)(H_U8);
typedef H_U32 (*get_pin_num)(void);
//typedef H_U32 (*wifi_wlan_connect)(void);



typedef struct near_control_device{
	H_U32 type;
	near_open _open;
	send_nonce _send_nonce;
	send_random _send_random;
	near_event _near_event;
	feedbakk _feedback;
	function_void _reset_random;
	near_control_nonce_data _nonce_data;
	is_connected _is_connected;
	uint32_function _control_type;
	wifi_ssid_store _ssid_store;
	wifi_pwd_store _pwd_store;
	wifi_type_store _type_store;
	get_pin_num _get_pin;
	//wifi_wlan_connect _wlan_connect;

}near_control_device_t;

near_control_device_t* wy_near_control_op(void);
near_control_manage_t* wy_near_control_manage_get(void);


#endif

