#include <config.h>
#include <wy_storage.h>
#include <wy_uart.h>
#include <wy_tools.h>
#include "lib_i2c.h"
#include "wy_control_common.h"
#include <wy_gpio.h>

#ifdef MODULE_TOUCH_KEYBOARD_ENABLE
#include "wy_touch.h"
#include "wy_keyboard_config.h"
#endif
#include "wy_system.h"

#define TOUCH_DEBUG			"TOUCH"

static struct touch_manage g_touch_manage;

static struct touch_manage* __touch_manage_get(void)
{
	return (struct touch_manage *) &g_touch_manage;
}

static void __touch_flash_callback(pstorage_handle_t * handle, H_U8 op_code, H_U32 result, 
														H_U8 * p_data, H_U32 data_len)
{
	WYPrintf("touch",LEVEL_DEBUG,"touch flash callback result:%d for op_code:%d",result,op_code);
		struct touch_manage *manage = __touch_manage_get();
		switch(op_code) 
		{ 
				case PSTORAGE_STORE_OP_CODE: 
				{
					//readback the stored key
					H_U8 stored_data[TOUCH_BLOCK_LEN];
					H_U16 magic;
					_StorageRead(&(manage->pg_handler), TOUCH_BLOCK_LEN, 0, stored_data,TOUCH_BLOCK_LEN);
					magic = decode16(stored_data,0);
					if(magic == TOUCH_MAGIC)//check the result of write
					{
						wy_memcpy(manage->default_code,&(stored_data[TOUCH_MAGIC_LEN]),TOUCH_BLOCK_LEN);
				 		WYPrintf_array(manage->default_code,PASSCODE_LENGTH);
					}//if not, do not overwrite the manage->default_code
				}
				break; 
						default: 
				break; 
		} 
} 



H_U32 _TouchOpen(void)
{
	H_U8 stored_data[TOUCH_BLOCK_LEN];
	H_U16 magic;
	
	wy_memset(&g_touch_manage, 0, sizeof(struct touch_manage));
	//×¢²á°´¼ü´æ´¢ÐÅÏ¢
	_StorageRegister(&(g_touch_manage.pg_handler), STORAGE_BLOCK_LEN, 1,__touch_flash_callback);
	
	//readback the password
	_StorageRead(&(g_touch_manage.pg_handler), TOUCH_BLOCK_LEN, 0, stored_data,TOUCH_BLOCK_LEN);
	magic = decode16(stored_data,0);
	if(magic == TOUCH_MAGIC)//indicate that there is a valid passcode,
	{
		wy_memcpy(&(g_touch_manage.default_code),&(stored_data[TOUCH_MAGIC_LEN]),PASSCODE_LENGTH);
		WYPrintf(TOUCH_MODULE,LEVEL_DEBUG,"valid passcode");
		WYPrintf_array(g_touch_manage.default_code,PASSCODE_LENGTH);
	}else{ //there is no valid passcode, use default passcode
		WYPrintf(TOUCH_MODULE,LEVEL_DEBUG,"use default passcode");
	}
	wy_system_op()->_touch_led_set();
	wy_tools_op()->_delay_ms(200);
	
#if (TOUCH_KEYBOARD_TYPE == TOUCH_KEYBOARD_NORMAL)
		nrf_gpio_cfg_output(TOUCH_LED_PIN);
		nrf_gpio_pin_clear(TOUCH_LED_PIN);
#elif (TOUCH_KEYBOARD_TYPE == TOUCH_KEYBOARD_TTY6952)
		m2mkey_tty6952_open();
#else
#endif
	wy_system_op()->_touch_led_clear();

	return H_SUCCESS;
}


static H_U32 __touch_code_update(H_U8* new_data)
{
	struct touch_manage *manage = __touch_manage_get();
	
	H_U32 errcode = 0;
    //use global variable for storing
	static H_U8 stored_pwd[TOUCH_BLOCK_LEN];
	wy_memset(stored_pwd,0,TOUCH_BLOCK_LEN);
	//add the magic data to the array
	encode16(TOUCH_MAGIC,stored_pwd,0);
	//copy the input data to the array
	wy_memcpy(&(stored_pwd[TOUCH_MAGIC_LEN]),new_data,PASSCODE_LENGTH);
	WYPrintf(TOUCH_MODULE,LEVEL_DEBUG,"before storing");
	WYPrintf_array(stored_pwd,TOUCH_BLOCK_LEN);
    errcode = _StorageErase(&manage->pg_handler,STORAGE_BLOCK_LEN);

	errcode = _StorageWrite(&manage->pg_handler,stored_pwd,TOUCH_BLOCK_LEN,0,TOUCH_BLOCK_LEN); 
	return errcode;
}
	

static H_U32 __touch_virtual_proc(H_U32 sw_key)
{
	WYPrintf(TOUCH_MODULE,LEVEL_DEBUG,"__touch_virtual_proc  sw_key:%d",sw_key);
	wy_system_op()->_touch_led_set();
	//TODO: do something
	switch(sw_key)
	{
		case _VIRTUAL_NUM0:
		case _VIRTUAL_NUM1:
		case _VIRTUAL_NUM2:
		case _VIRTUAL_NUM3:
		case _VIRTUAL_NUM4:
		case _VIRTUAL_NUM5:
		case _VIRTUAL_NUM6:
		case _VIRTUAL_NUM7:
		case _VIRTUAL_NUM8:
		case _VIRTUAL_NUM9:
			break;
		case _VIRTUAL_NUMX:
			break;
		case _VIRTUAL_NUMH:
			break;
		default:
			break;
	}
	return sw_key;
}

static H_U32 __touch_key_proc(H_U32 hd_key)
{
	touch_table_t key_proc[] = TOUCH_VALUE_TABLE;
	H_U32 proc_size = wy_array_len(key_proc);
	for(H_U32 i = 0; i < proc_size; i++)
	{
		if(hd_key == key_proc[i].hd_key)
		{
			__touch_virtual_proc(key_proc[i].sw_key);
			return key_proc[i].sw_key;
		}
	}
	return H_FAULTER;
}

#if (TOUCH_KEYBOARD_TYPE == TOUCH_KEYBOARD_NORMAL)
static H_U16 __touch_normal_obtain(void)
{
	H_U8 pin_number;	  
 	H_U8 addr;
 	return lib_i2c_read16(TOUCH_NORMAL_ADDR, addr);
}
#endif


H_U32 _TouchRead(void)
{  
	H_U32 key_number = TOUCH_ERROR_KEY;
#if (TOUCH_KEYBOARD_TYPE == TOUCH_KEYBOARD_NORMAL)
	key_number = __touch_normal_obtain();
#elif (TOUCH_KEYBOARD_TYPE == TOUCH_KEYBOARD_TTY6952)
	key_number = m2mkey_tty6952_obtain();

#else 
	key_number = TOUCH_ERROR_KEY;
#endif
	WYPrintf(TOUCH_MODULE,LEVEL_DEBUG,"__touch_key_read  hw_key:%x",key_number);
	if(key_number != TOUCH_ERROR_KEY)
	{
		return __touch_key_proc(key_number);
	}
	return H_FAULTER;
}


