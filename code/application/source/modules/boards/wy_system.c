#include <wy_storage.h>
#include <nrf_gpio.h>
#include <Nrf_soc.h>
#include <boards.h>
#include "wy_tools.h"
#include "wy_control_common.h"
#include "wy_system.h"
#include "wy_uart.h"
#include "wy_event.h"
#include "wy_battery.h"
#include "wy_alarm.h"
#include "wy_light.h"
#ifdef MODULE_TOUCH_KEYBOARD_ENABLE
#include "wy_touch.h"
#endif


static struct system_manage g_system_manage;

static struct system_manage* __system_manage_get(void)
{
	return (struct system_manage *)&g_system_manage;
}

/*

static void __system_data_load(void)
{
	H_U32 ret_code = 0;
	struct system_manage* manage = __system_manage_get();
	ret_code = _StorageRead(&manage->pg_handler,sizeof(manage->data),0,(H_U8 *)&(manage->data),sizeof(manage->data));
	WYPrintf(SYSTEM_MODULE,LEVEL_DEBUG,"block:%d; module:%d",manage->pg_handler.block_id,manage->pg_handler.module_id);
	if((ret_code != H_SUCCESS) || (manage->data.magic != SYSTEM_MAGIC_TURN))
	{
		manage->data.mode_switch = LOCK_MODE_FAMILY;
	}
	manage->data.lock_state = nrf_gpio_pin_read(LOCK_STATE_DETECTION);
	manage->data.door_state = nrf_gpio_pin_read(DOOR_STATE_DETECTION);
	manage->data.lock_state_prev = g_system_manage.data.lock_state;
	manage->data.door_state_prev = g_system_manage.data.door_state;
	manage->data.add_lock_prompt = 0;
	manage->data.voice.voice_played = 0;
	WYPrintf(SYSTEM_MODULE,LEVEL_DEBUG,"system mode_switch :%d magic:%x , mode:%d",manage->data.mode_switch,manage->data.magic,manage->data.mode_switch);
	
}

static void __system_data_save(void)
{
	struct system_manage* manage = __system_manage_get();
	device_storage_gather_t device_info;
	device_info.handler = &(manage->pg_handler);
	device_info.id = 0;
	device_info.hand_len = SYSTEM_MAGIC_LEN;
	device_info.data = &(manage->data.lock_state);
	device_info.data_len = (sizeof(manage->data) - SYSTEM_MAGIC_LEN);
	device_info.user_block_size = sizeof(manage->data);
	device_info.storage_counter = 1;
	device_info.magic_clear = H_FALSE;
	device_info.magic_data = SYSTEM_MAGIC_DATA;
	_StorageUpdata(&device_info);
}
*/


static void __system_flash_callback(pstorage_handle_t * handle, H_U8 op_code, H_U32 result, 
														H_U8 * p_data, H_U32 data_len)
{ 
		switch(op_code) 
		{ 
				case PSTORAGE_STORE_OP_CODE: 
				break; 
						default: 
				break; 
		} 
} 



static H_U32 __system_open(void)
{
	wy_memset(&g_system_manage, 0, sizeof(struct system_manage));
	_StorageRegister(&(g_system_manage.pg_handler), STORAGE_BLOCK_LEN, 1,__system_flash_callback);
	nrf_gpio_cfg_input(LOCK_STATE_DETECTION,GPIO_PIN_CNF_PULL_Disabled);
	nrf_gpio_cfg_input(DOOR_STATE_DETECTION,GPIO_PIN_CNF_PULL_Disabled);

	//__system_data_load();
	
	g_system_manage._system_timer._one_wire_interval = 0;
	g_system_manage._system_timer._battery_measure_interval = 0;
	g_system_manage._system_timer._connection_limit = 0;
	g_system_manage._system_timer._auto_lock_interval = 0;
	g_system_manage._system_timer._add_lock_prompt_interval = 0;
	g_system_manage._system_timer._alarm_on_interval = 0;
	#ifdef MODULE_TOUCH_KEYBOARD_ENABLE
	g_system_manage._system_timer._touch_key_led_interval = 0;
	#endif
	return H_SUCCESS;
}


#ifdef  MODULE_TOUCH_KEYBOARD_ENABLE
static void __touch_led_clear(void)
{
	
#if (TOUCH_LED_TYPE == TOUCH_LED_GUIDE)
		_LightOnOff(_LIGHT_BIG,H_FALSE);
#elif (TOUCH_LED_TYPE == TOUCH_LED_INDEPENDENT) 
		_LightOnOff(_LIGHT_LED1,H_FALSE);
#else
#endif

}

static void __touch_led_interval_set(void)
{
	struct system_manage* manage = __system_manage_get();
	manage->_system_timer._touch_key_led_interval = 30;//10s
	manage->_system_timer._touch_restart = 0;
	//nrf_gpio_pin_set(TOUCH_LED_PIN);
#if (TOUCH_LED_TYPE == TOUCH_LED_GUIDE)
	_LightOnOff(_LIGHT_BIG,H_TRUE);
#elif (TOUCH_LED_TYPE == TOUCH_LED_INDEPENDENT) 
	_LightOnOff(_LIGHT_LED1,H_TRUE);
#else
#endif
	
}


#endif

static H_U32 __system_reboot(void)
{
	APP_ERROR_CHECK((~0));/*重启系统*/
	return 0;
}

static H_U32 __system_factory_set(void)
{
	H_U32 ret_code = 0;

	wy_near_control_op()->_feedback(CTL_BACK_POWER_RESTART,H_NULL,0);
	wy_tools_op()->_delay_ms(500);
	__system_reboot();/*重启系统*/
	return H_SUCCESS;
}

static void __sys_battery_process(void)
{
	struct system_manage* manage = __system_manage_get();
	manage->_system_timer._battery_measure_interval++;
	if(manage->_system_timer._battery_measure_interval >= BATTERY_MEASURE_ON_TIMER_COMMON)
	{
		manage->_system_timer._battery_measure_interval =0;
		wy_battery_op()->_get_level();
	}
}



#ifdef MODULE_TOUCH_KEYBOARD_ENABLE
static void __sys_touch_process(void)
{
	struct system_manage* manage = __system_manage_get();
	if(manage->_system_timer._touch_key_led_interval > 0)
	{
		manage->_system_timer._touch_key_led_interval--;
		if(manage->_system_timer._touch_key_led_interval == 0)
		{
#if (TOUCH_LED_TYPE == TOUCH_LED_GUIDE)
			_LightOnOff(_LIGHT_BIG,H_FALSE);
#elif (TOUCH_LED_TYPE == TOUCH_LED_INDEPENDENT) 
			_LightOnOff(_LIGHT_LED1,H_FALSE);
#endif
		}
	}
}

static void __sys_touch_restart(void)
{
	struct system_manage* manage = __system_manage_get();
	extern H_U32 m2mkey_tty6952_open(void);
	
	manage->_system_timer._touch_restart++;
	if(manage->_system_timer._touch_restart > FINGER_MODE_TIMER_COMMON)
	{
		manage->_system_timer._touch_restart = 0;
		m2mkey_tty6952_open();
	}
}
#endif

//timer_common callback
static H_U32 __system_timer_process(void)
{
	struct system_manage* manage = __system_manage_get();

	//battery
	__sys_battery_process();
	

//touch led close
#ifdef MODULE_TOUCH_KEYBOARD_ENABLE
	__sys_touch_process();
	//__sys_touch_restart();
#endif
	//autolock interval

	return H_SUCCESS;
}

static struct system_data *__system_data_get(void)
{
	return (struct system_data *)&g_system_manage.data;
}

system_device_t* wy_system_op(void)
{
	static system_device_t ___system_op = {
			._open = __system_open,
			._system_reset = __system_factory_set,
			._system_reboot = __system_reboot,
			._system_timer_common_handle = __system_timer_process,
		#ifdef MODULE_TOUCH_KEYBOARD_ENABLE
			._touch_led_clear = __touch_led_clear,
			._touch_led_set = __touch_led_interval_set,
		#endif
			._sys_get = __system_data_get,
	};
	return &___system_op;
}


