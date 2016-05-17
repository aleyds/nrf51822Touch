
#ifndef __TOOLS_H__
#define __TOOLS_H__
#include <base_type.h>
#include <nrf_delay.h>
#include <wy_aes_cipher.h>


#define TOOLS_DEBUG 1
#ifdef TOOLS_DEBUG
#else 
#endif

#define BOOTLOADER_DFU_START				(0x01)
#define BOOTLOADER_DFU_STOP					(0x00)

typedef void (*delay)(H_U32 volatile time);
typedef H_U8 (*random_byte)(void);
typedef H_S32 (*random_any)(H_U8 *data, H_U32 len);
typedef H_S32 (*to_string)(const H_U8 *, const H_U16  , H_U8 * , H_U16 );
typedef H_U32 (*tools_difference)(H_U32, H_U32);


typedef struct tools_device{
	H_U32 type;
	delay _delay_ms;
	delay _delay_us;
	delay _sleep;
	random_byte _random_byte;
	random_any _random_any;
	function_void _power_manage;
	uint32_function_uint32 _fibonacci;
	tools_difference _difference;
	function_void _notice;
	to_string _to_str;
	uint32_function _dfu_mode;
	aes_cipher_t _cipher_aes;
}tools_device_t;

tools_device_t* wy_tools_op(void);

#endif

