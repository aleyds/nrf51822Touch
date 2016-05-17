#ifndef __HS_TOUCH_H__
#define __HS_TOUCH_H__
#include <base_type.h>
#include <wy_storage.h>
//#include "Pca10001.h"

#define MAX_INPUT_LENGTH  10
#define PASSCODE_LENGTH    6
#define TOUCH_MAGIC_LEN    2
#define TOUCH_BLOCK_LEN    PASSCODE_LENGTH + TOUCH_MAGIC_LEN
#define TOUCH_LED_PIN      LIGHT_LED_BIG_PORT
#define TOUCH_MAGIC        0xA074


#define TOUCH_MODULE  "TOUCH"

#define TOUCH_NORMAL_ADDR			(0xAF)

struct touch_manage{
	pstorage_handle_t pg_handler;
	H_U8 input[MAX_INPUT_LENGTH];   //store the input 
	H_U32 cur_pointer; //indicate the position the next input should be placed in the input 
	H_U8  default_code[PASSCODE_LENGTH]; //test
};


typedef H_U32 (*touch_key_read)(void);
typedef H_U32 (*touch_open)(void);
typedef H_U32 (*key_modify)(H_U8*);

typedef struct touch_key_device{
	touch_open _open;
	touch_key_read _read;	
	key_modify _modify;
	function_void _reset;
}touch_key_device_t;

H_U32 _TouchOpen(void);
H_U32 _TouchRead(void);



#endif

