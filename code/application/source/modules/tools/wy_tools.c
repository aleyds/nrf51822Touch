
#include <wy_tools.h>
#include <boards.h>
#include <nrf.h>
#include <nrf_gpio.h>
#include <app_error.h>
#include <nrf_soc.h>
#include "wy_alarm.h"


static void __tools_delay_ms(H_U32 volatile ms)
{
	while(ms != 0)
	{
		ms--;
		nrf_delay_us(999);
	}
}

static void __tools_delay_us(H_U32 volatile us)
{
	nrf_delay_us(us);
}


static H_U8 __tools_random_byte(void)
{
	H_S32 timeout = 100;
	NRF_RNG->TASKS_START = 1; // start the RNG peripheral.
	NRF_RNG->EVENTS_VALRDY = 0;
	while (NRF_RNG->EVENTS_VALRDY == 0)
	{
		// Do nothing.
		if(timeout <= 0)
		{
			break;
		}
		timeout--;
		__tools_delay_ms(1);
	}
	NRF_RNG->TASKS_STOP = 1;
	return	(H_U8)NRF_RNG->VALUE;
}


static H_S32 __tools_random_any(H_U8 *data, H_U32 len)
{
	memset(data, 0, len*sizeof(H_U8));
	for(H_U32 i = 0; i < len; i++)
	{
		data[i] = __tools_random_byte();
		__tools_delay_us(100);
	}		
	return 0;
}


/*
  Calculation  value1 and value2 difference
*/
static H_U32 __tools_difference(H_U32 value1 , H_U32 value2)
{
	if(value1 == value2)
	{
		return 0;
	}
	else if(value1 > value2)
	{
		return (H_U32)(value1 - value2);
	}
	else 
	{
		return (H_U32)(value2 - value1);
	}
}


static H_S32 __tools_array_to_string(const H_U8 *array, const H_U16 array_len , H_U8 *str , H_U16 str_len)
{
	H_U8 *p_anscii = "0123456789ABCDEF";
	if(str_len < (array_len<<1))
	{
		return H_FAULTER;
	}
	for(int i = 0; i < array_len; i++)
	{
		str[i<<1] = p_anscii[(array[i]>>4)&0xf];
		str[(i<<1) + 1] = p_anscii[array[i]&0xf];
	}
	return H_SUCCESS;
}


static void __tools_power_manage(void)
{
   
    H_U32 err_code = sd_app_evt_wait();
    
    APP_ERROR_CHECK(err_code);
}


static void __tool_sleep(H_U32 ms)
{
	while(ms != 0)
	{
		ms--;
		sd_app_evt_wait();
	}
}

static H_U32 __tool_fibonacci(H_U32 n)
{
	H_U32 ret_code = 0;
	if(n == 0)
	{
		ret_code = 0;
	}
	else if(n == 1)
	{
		ret_code = 1;
	}
	else
	{
		ret_code = __tool_fibonacci(n - 1) + __tool_fibonacci(n - 2);
	}
	return ret_code;
}

static H_U32 __trigger_dfu_mode(void)
{
	H_U32 err_code = sd_power_gpregret_set(BOOTLOADER_DFU_START);
	if(err_code != H_SUCCESS)
	{
		return H_FAULTER;
	}
	APP_ERROR_CHECK((~0));
	return H_SUCCESS;
}


static void __tool_user_notice(void)
{
#ifdef MODULE_ALARM_ENABLE
	wy_alarm_op()->_port_onoff(H_TRUE);
#endif

}

tools_device_t* wy_tools_op(void)
{
	static tools_device_t ___tools_op = {
				._delay_ms = __tools_delay_ms,
				._delay_us = __tools_delay_us,
				._sleep = __tool_sleep,
				._random_byte = __tools_random_byte,
				._random_any = __tools_random_any,
				._power_manage = __tools_power_manage,
				._fibonacci = __tool_fibonacci,
				._difference = __tools_difference,
				._notice = __tool_user_notice,
				._to_str = __tools_array_to_string,
				._dfu_mode = __trigger_dfu_mode,
				._cipher_aes = {
						._setkeyiv = wy_cipher_setkeyiv,
						._encrypt = wy_cipher_encrypt,
						._decrypt = wy_cipher_decrypt,
				},
	};
	return &___tools_op;
}


