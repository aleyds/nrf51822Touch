#include <app_error.h>
#include <softdevice_handler.h>
#include <ble_dis.h>
#include <ble_nus.h>
#include <ble_conn_params.h>
#include <wy_ble_common.h>
#include <wy_tools.h>
#include <wy_uart.h>
#include "wy_alarm.h"
#include <wy_control_common.h>
#include <wy_control_core.h>
#include "wy_system.h"
#include "wy_battery.h"

static near_control_manage_t g_control_manage;
static void __near_nus_data_handler(ble_nus_t * p_nus, H_U8 * p_data,data_type mode,H_U8 type);
static H_S32 __near_control_feedback(struct ctl_feedback *back);
static H_U32 __connect_wlan(void);
static H_U32 __near_control_is_connected(void);

static struct bluetooth_command g_ble_command[]=
{
	{
		.cmd_type = CTL_GET_LOCK_INFO,
		.feedback_type = CTL_BACK_LOCK_INFO,
		.access = CTL_ACCESS_ALL,
		._cmd_cb = wy_near_get_stauts,
		._bk_cb = __near_control_feedback,
	},

#ifdef MODULE_ALARM_ENABLE
	{
		.cmd_type = CTL_SOUND_ALARM,
		.feedback_type = CTL_BACK_ALARM_STATUS,
		.access = CTL_ACCESS_ALL,
		._cmd_cb = wy_near_alarm,
		._bk_cb = H_NULL,//let alarm module feedback
	},
	{
		.cmd_type = CTL_DISALARM,
		.feedback_type = CTL_BACK_ALARM_STATUS,
		.access = CTL_ACCESS_ALL,
		._cmd_cb = wy_near_alarm,
		._bk_cb = H_NULL,//let alarm module feedback
	},
#endif

#ifdef MODULE_TOUCH_KEYBOARD_ENABLE
{
		.cmd_type = CTL_TOUCHKEY_MODIFY,
		.feedback_type = CTL_BACK_TOUCHKEY_MODIFY,
		.access = CTL_ACCESS_ADMIN,
		._cmd_cb = wy_touch_key_modify,
		._bk_cb = __near_control_feedback,
	},
#endif

 #ifdef MODULE_DFU_ENABLE
 	{
		.cmd_type = CTL_ENTER_DFU,
		.feedback_type = CTL_BACK_ENTER_DFU,
		.access = CTL_ACCESS_ADMIN,
		._cmd_cb = m2mkey_dfu_enter,
		._bk_cb = H_NULL,
 	},
 #endif
};
#define BLE_COMMAND_SIZE	(sizeof(g_ble_command)/sizeof(g_ble_command[0]))


static H_U8 g_macsalt[CTL_COMMAND_DATA_LEN] = {0x4d,0x96,0xc1,0xc5,0x7d,0xc7,0x9d,0x99,
				  					  		   0x17,0xaf,0x84,0xd2,0xd0,0xeb,0x91,0x18};
											
static H_U8 g_msgsalt[CTL_COMMAND_DATA_LEN] = {0x34,0x71,0x25,0xa6,0x38,0x2b,0xe9,0x50,
				  					  		   0xe2,0xee,0x81,0x4a,0x6c,0x32,0x1a,0x66};
											
static H_U8 g_addkeysalt[CTL_COMMAND_DATA_LEN] = {0xa4,0xfc,0xe3,0xa0,0x07,0x8e,0x47,0xfd,
					 							  0xac,0xff,0xf1,0x51,0xf3,0xaa,0xc4,0xc7};


static LIST_HEAD(g_ble_control);

near_control_manage_t* wy_near_control_manage_get(void)
{
	return (near_control_manage_t*)&g_control_manage;
}

static H_S32 __near_control_ctl(struct ctl_command *op)
{
	CHECK_PARAMETER(op,H_NULL,H_FAULTER);
	struct bluetooth_command *pos;
	struct ctl_feedback feedbask;
	near_control_manage_t* manage = wy_near_control_manage_get();
	wy_memset(&feedbask, 0, sizeof(struct ctl_feedback));
	WYPrintf(MODULE_NEAR_CONTROL,LEVEL_DEBUG," op_type:%d  ctl_type:%d",op->type,op->control_type);
	
	list_for_each_entry(pos, &g_ble_control, entries,struct bluetooth_command)
	{
		if(H_NULL != pos)
		{
			if((pos->cmd_type == op->type) && ((pos->access & op->access) != 0))
			{
				pos->_cmd_cb(op, &feedbask);
				
				feedbask.type = pos->feedback_type;
				
				if(pos->_bk_cb != H_NULL)//let other module do the notify
				{
					pos->_bk_cb(&feedbask);
				}
				return H_SUCCESS;
			}
		}
	}
	return H_FAULTER;// to-do: counter check missed 
}

static H_S32 __near_control_convert(const H_U8 *org_data,struct ctl_command *cmd_op)
{
	CHECK_PARAMETER(org_data,H_NULL,H_FAULTER);
	CHECK_PARAMETER(cmd_op,H_NULL,H_FAULTER);
	near_control_manage_t* manage = wy_near_control_manage_get();

	wy_memset(cmd_op, 0, sizeof(struct ctl_command));
	cmd_op->counter = decode_turn16(org_data,0);
	//manage->ctl_counter = cmd_op->counter;
	cmd_op->type = decode_turn16(org_data,2);
	cmd_op->access =  manage->access;
	wy_memcpy(cmd_op->data, &(org_data[4]), CTL_FEEDBACK_DATA_LEN - CTL_HAND_DATA_LEN);
	WYPrintf(MODULE_NEAR_CONTROL,LEVEL_DEBUG,"COUNTER FROM APP:%d",cmd_op->counter);
	return H_SUCCESS;
}

static H_S32 __near_control_nonce_data(H_U8 *nonce_cleartext)
{
	near_control_manage_t* manage = wy_near_control_manage_get();
	H_U8 buffer[CTL_COMMAND_DATA_LEN + CTL_MAC_DATA_LEN] = { 0 };
	H_U16 lenght = CTL_COMMAND_DATA_LEN + CTL_MAC_DATA_LEN;
	
	ble_nus_nonce_get(&(manage->handler), buffer, &lenght);
	__near_control_cleartext(buffer, nonce_cleartext);
	return H_SUCCESS;
}

static H_S32 __near_control_cleartext(const H_U8 * ciphertext, H_U8 *cleartext)
{

	CHECK_PARAMETER(ciphertext,H_NULL,H_FAULTER);
	CHECK_PARAMETER(cleartext,H_NULL,H_FAULTER);
	near_control_manage_t* manage = wy_near_control_manage_get();
	H_U8 msg_cipher[CTL_COMMAND_DATA_LEN] = {0};
	H_U8 mac_cipher[CTL_MAC_DATA_LEN] = {0};
	H_U8 tmp[CTL_COMMAND_DATA_LEN];

	wy_memcpy(&(msg_cipher[0]), ciphertext, CTL_COMMAND_DATA_LEN);
	wy_memcpy(&(mac_cipher[0]), &(ciphertext[16]), CTL_MAC_DATA_LEN);

	wy_tools_op()->_cipher_aes._setkeyiv(AES_ECB_NORMAL_TYPE,AES_CIPHER_LENGHT,manage->mac_sesskey,manage->init_vector);
	wy_tools_op()->_cipher_aes._encrypt(msg_cipher,tmp);

	if(wy_memcmp(mac_cipher,tmp,CTL_MAC_DATA_LEN) == 0)
	{
		wy_tools_op()->_cipher_aes._setkeyiv(AES_ECB_NORMAL_TYPE,AES_CIPHER_LENGHT,manage->msg_sesskey,manage->init_vector);
		wy_tools_op()->_cipher_aes._decrypt(msg_cipher,cleartext);
	}
	return H_SUCCESS;
}


static H_S32 __near_control_register(struct bluetooth_command *cmd)
{
	CHECK_PARAMETER(cmd,H_NULL,H_FAULTER);
	list_add_tail(&cmd->entries, &g_ble_control);
	return H_SUCCESS;
}

static H_U32 __near_control_type(void)
{
	H_U32 isconnect = __near_control_is_connected();
	
	if(isconnect != H_FAULTER)
	{
		return _TYPE_BLE_CONTROL;
	}
	else
	{
		return _TYPE_CONTROL_MAX;
	}
}

static H_S32 __near_control_back_general(H_U32 type, const H_U8 *data,H_U32 len)                                                                                                           
{
	struct ctl_feedback feedback_data;
	near_control_manage_t* manage = wy_near_control_manage_get();

	wy_memset(&feedback_data, 0, sizeof(struct ctl_feedback));
	feedback_data.control_type = __near_control_type();
	if(feedback_data.control_type == _TYPE_CONTROL_MAX)
	{
		return H_FAULTER;
	}
	
	if(len > CTL_FEEDBACK_DATA_LEN - CTL_HAND_DATA_LEN)
	{
		WYPrintf(MODULE_NEAR_CONTROL, LEVEL_ERROR, "Back Data Lenght is large");
		return H_FAULTER;
	}
	
	feedback_data.type = type;
	//manage->ctl_counter += 1;
	//feedback_data.counter = manage->ctl_counter;
	
	WYPrintf(MODULE_NEAR_CONTROL,LEVEL_DEBUG,"FEEDBACK COUNTER:%d",feedback_data.counter);

	feedback_data.back_flag = FEEDBACK_STATE;
	if((data != H_NULL) || (len != 0))
	{
		wy_memcpy(feedback_data.data.extra_data.data, data,len);
	}
	__near_control_feedback(&feedback_data);
	return H_SUCCESS;
}

extern H_U32 wy_accord_cmd(H_U32 answer ,H_U8 *cmd, H_U16 len);

static H_S32 __near_control_feedback(struct ctl_feedback *back)
{
	CHECK_PARAMETER(back,H_NULL,H_FAULTER);
	near_control_manage_t* manage = wy_near_control_manage_get();
	//counter update
	manage->ctl_counter += 1;
	back->counter = manage->ctl_counter;
	H_U8 feedback_data[CTL_FEEDBACK_DATA_LEN] = {0};
	H_U8 msg_encrypt[CTL_FEEDBACK_DATA_LEN] = {0};
	H_U8 mac_encrypt[CTL_FEEDBACK_DATA_LEN] = {0};
	H_U8 send_data[CTL_FEEDBACK_DATA_LEN + CTL_MAC_DATA_LEN] = {0};
	H_U32 err_code = 0;

	wy_memset(feedback_data,0 , sizeof(feedback_data));
	encode_turn16(back->counter,feedback_data,0);
	encode_turn16(back->type,feedback_data,2);
	
	switch(back->type)
	{
		case CTL_BACK_LOCK_INFO:
		case CTL_BACK_SWITCH_CHANGE:
		case CTL_BACK_LOCK_STATUS:
		case CTL_BACK_BATTERY:
		case CTL_BACK_ALARM_STATUS:
		case CTL_BACK_MOTOR_STATUS:
		case CTL_BACK_SKEY_MODE:
#ifdef MODULE_ALARM_ENABLE		
			back->data.info.alarm_state = wy_alarm_op()->_status_get();
#endif
			back->data.info.battery_level = wy_battery_op()->_get_level_direct();
			back->data.info.door_state = wy_system_op()->_door_state();		
			back->data.info.lock_state = wy_system_op()->_lock_state();
			back->data.info.mode_switch = wy_system_op()->_mode_get();
			feedback_data[4] = (back->data.info.battery_level)&0xff;
			feedback_data[5] = ((back->data.info.lock_state) | (back->data.info.door_state << 1) | (back->data.info.alarm_state<< 2)\
								| (back->data.info.mode_switch << 3) | (back->data.info.skey_mode << 4) | (back->data.info.motor_state << 5)\
								| (back->data.info.time_sync << 6))&0xff;
				
			WYPrintf(MODULE_NEAR_CONTROL,LEVEL_DEBUG,"battery:%d  status all:%b",feedback_data[4],feedback_data[5]);
		break;	
	
		default:
			wy_memcpy((H_U8 *)(&(feedback_data[4])),back->data.extra_data.data,CTL_FEEDBACK_DATA_LEN - CTL_HAND_DATA_LEN);
		break;
	}
	WYPrintf(MODULE_NEAR_CONTROL,LEVEL_DEBUG,"final data:");
	WYPrintf_array(feedback_data,16);
	wy_tools_op()->_cipher_aes._setkeyiv(AES_ECB_NORMAL_TYPE,AES_CIPHER_LENGHT,manage->msg_sesskey,manage->init_vector);
	wy_tools_op()->_cipher_aes._encrypt(feedback_data,msg_encrypt);

	wy_tools_op()->_cipher_aes._setkeyiv(AES_ECB_NORMAL_TYPE,AES_CIPHER_LENGHT,manage->mac_sesskey,manage->init_vector);
	wy_tools_op()->_cipher_aes._encrypt(msg_encrypt,mac_encrypt);

	wy_memcpy((H_U8 *)(&(send_data[0])),msg_encrypt,CTL_FEEDBACK_DATA_LEN);
	wy_memcpy((H_U8 *)(&(send_data[CTL_FEEDBACK_DATA_LEN])),mac_encrypt,CTL_MAC_DATA_LEN);
	if(back->control_type == _TYPE_BLE_CONTROL)
	{
		err_code = ble_nus_send_status(&(manage->handler),send_data,CTL_FEEDBACK_DATA_LEN + CTL_MAC_DATA_LEN);
	}

	return H_SUCCESS;
}


static void __near_nus_data_handler(ble_nus_t * p_nus, H_U8 * p_data,data_type mode, H_U8 type)
{
	near_control_manage_t* manage = wy_near_control_manage_get();
	switch(mode)
	{
		case _TYPE_COMMAND:
			{
				
			}
			break;
		case _TYPE_NONCE:
			break;
		case _TYPE_SESSION:
			{
			
			}
			break;
	}
}


static void __near_reset_random(void)
{
	near_control_manage_t* manage = wy_near_control_manage_get();
	if(manage != H_NULL)
	{
		wy_tools_op()->_random_any(manage->random,CTL_COMMAND_DATA_LEN);
	}
}
//global variable init
static void __near_control_open(void)
{

	near_control_manage_t* manage = wy_near_control_manage_get();
	wy_memset(manage, 0,sizeof(near_control_manage_t));
	
	wy_tools_op()->_random_any(manage->random,CTL_COMMAND_DATA_LEN);
	wy_bluetooth_op()->_character._start(&(manage->handler),__near_nus_data_handler,manage->random);
	
	manage->macsalt = g_macsalt;
	manage->msgsalt = g_msgsalt;
	manage->addkeysalt = g_addkeysalt;
	manage->user_pin_num = -1;//invalid user no.
	for(H_U32 i = 0; i < BLE_COMMAND_SIZE;i ++)
	{
		__near_control_register((struct bluetooth_command *)&(g_ble_command[i]));
	}
	manage->wifi_ssid.ssid_len = 0;
	wy_memset(manage->wifi_ssid.ssid,0,wy_strlen(manage->wifi_ssid.ssid));
	manage->wifi_password.pwd_len = 0;
	wy_memset(manage->wifi_password.pwd, 0, wy_strlen(manage->wifi_password.pwd));
	manage->wifi_type = 0xFF;

}

static H_S32 __near_control_send_nonce( H_U8 *data, H_U32 len)
{
	near_control_manage_t* manage = wy_near_control_manage_get();
	ble_nus_send_nonce(&manage->handler,data,len);
	return H_SUCCESS;
}

static void __near_control_send_random(void)
{
	near_control_manage_t* manage = wy_near_control_manage_get();
	ble_nus_send_nonce(&manage->handler,manage->random,CTL_COMMAND_DATA_LEN);
}

static void __near_control_event(ble_evt_t * p_ble_evt)
{
	near_control_manage_t* manage = wy_near_control_manage_get();
	ble_nus_on_ble_evt(&manage->handler, p_ble_evt);
}

static H_U32 __near_control_is_connected(void)
{
	if(wy_bluetooth_op()->_isconnect())
	{
		return H_SUCCESS;
	}
	return H_FAULTER;
}




near_control_device_t* wy_near_control_op(void)
{
	static near_control_device_t ___near_control_op = {
		._open = __near_control_open,
		._send_nonce = __near_control_send_nonce,
		._send_random = __near_control_send_random,
		._near_event = __near_control_event,
		._feedback = __near_control_back_general,
		._reset_random = __near_reset_random,
		._nonce_data = __near_control_nonce_data,
		._is_connected = __near_control_is_connected,
		._control_type = __near_control_type,
	};
	return &___near_control_op;
}


