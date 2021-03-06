#ifndef __KEYBOARD_CFG_H__
#define __KEYBOARD_CFG_H__
#include "config.h"
#include "wy_touch_tty6952.h"
#include "wy_touch_button.h"
#include "wy_touch_bs83b12a3.h"

#define TOUCH_ERROR_KEY			(0x0000)

enum touch_value{
	_TOUCH_VALUE_NULL,
	_TOUCH_VALUE_NUM0 = 0x0004,
	_TOUCH_VALUE_NUM1 = 0x0080,
	_TOUCH_VALUE_NUM2 = 0x2000,
	_TOUCH_VALUE_NUM3 = 0x4000,
	_TOUCH_VALUE_NUM4 = 0x0040,
	_TOUCH_VALUE_NUM5 = 0x1000,
	_TOUCH_VALUE_NUM6 = 0x8000,
	_TOUCH_VALUE_NUM7 = 0x0020,
	_TOUCH_VALUE_NUM8 = 0x0008,
	_TOUCH_VALUE_NUM9 = 0x0001,
	_TOUCH_VALUE_NUMX = 0x0010,
	_TOUCH_VALUE_NUMH = 0x0002,
};

enum Virlual_key{
	_VIRTUAL_NUM0 = 0,
	_VIRTUAL_NUM1,
	_VIRTUAL_NUM2,
	_VIRTUAL_NUM3,
	_VIRTUAL_NUM4,
	_VIRTUAL_NUM5,
	_VIRTUAL_NUM6,
	_VIRTUAL_NUM7,
	_VIRTUAL_NUM8,
	_VIRTUAL_NUM9,
	_VIRTUAL_NUM10,
	_VIRTUAL_NUMX,
	_VIRTUAL_NUMH,
};

typedef struct touch_table{
	H_U32 hd_key;
	H_U32 sw_key;
}touch_table_t;

#if (TOUCH_KEYBOARD_TYPE == TOUCH_KEYBOARD_NORMAL)
#define TOUCH_VALUE_TABLE \
{\
	{_TOUCH_VALUE_NUM0, _VIRTUAL_NUM0},\
	{_TOUCH_VALUE_NUM1, _VIRTUAL_NUM1},\
	{_TOUCH_VALUE_NUM2, _VIRTUAL_NUM2},\
	{_TOUCH_VALUE_NUM3, _VIRTUAL_NUM3},\
	{_TOUCH_VALUE_NUM4, _VIRTUAL_NUM4},\
	{_TOUCH_VALUE_NUM5, _VIRTUAL_NUM5},\
	{_TOUCH_VALUE_NUM6, _VIRTUAL_NUM6},\
	{_TOUCH_VALUE_NUM7, _VIRTUAL_NUM7},\
	{_TOUCH_VALUE_NUM8, _VIRTUAL_NUM8},\
	{_TOUCH_VALUE_NUM9, _VIRTUAL_NUM9},\
	{_TOUCH_VALUE_NUMX, _VIRTUAL_NUMX},\
	{_TOUCH_VALUE_NUMH, _VIRTUAL_NUMH},\
};
#elif (TOUCH_KEYBOARD_TYPE == TOUCH_KEYBOARD_TTY6952)
#define TOUCH_VALUE_TABLE \
{\
	{_TOUCH6952_VALUE_NUM0, _VIRTUAL_NUM0},\
	{_TOUCH6952_VALUE_NUM1, _VIRTUAL_NUM1},\
	{_TOUCH6952_VALUE_NUM2, _VIRTUAL_NUM2},\
	{_TOUCH6952_VALUE_NUM3, _VIRTUAL_NUM3},\
	{_TOUCH6952_VALUE_NUM4, _VIRTUAL_NUM4},\
	{_TOUCH6952_VALUE_NUM5, _VIRTUAL_NUM5},\
	{_TOUCH6952_VALUE_NUM6, _VIRTUAL_NUM6},\
	{_TOUCH6952_VALUE_NUM7, _VIRTUAL_NUM7},\
	{_TOUCH6952_VALUE_NUM8, _VIRTUAL_NUM8},\
	{_TOUCH6952_VALUE_NUM9, _VIRTUAL_NUM9},\
	{_TOUCH6952_VALUE_NUMX, _VIRTUAL_NUMX},\
	{_TOUCH6952_VALUE_NUMH, _VIRTUAL_NUMH},\
};
#elif (TOUCH_KEYBOARD_TYPE == TOUCH_KEYBOARD_BUTTON)
#define TOUCH_VALUE_TABLE \
{\
	{_TOUCH_BUTTON_VALUE_NUM0, _VIRTUAL_NUM0},\
	{_TOUCH_BUTTON_VALUE_NUM1, _VIRTUAL_NUM1},\
	{_TOUCH_BUTTON_VALUE_NUM2, _VIRTUAL_NUM2},\
	{_TOUCH_BUTTON_VALUE_NUM3, _VIRTUAL_NUM3},\
	{_TOUCH_BUTTON_VALUE_NUM4, _VIRTUAL_NUM4},\
	{_TOUCH_BUTTON_VALUE_NUM5, _VIRTUAL_NUM5},\
	{_TOUCH_BUTTON_VALUE_NUM6, _VIRTUAL_NUM6},\
	{_TOUCH_BUTTON_VALUE_NUM7, _VIRTUAL_NUM7},\
	{_TOUCH_BUTTON_VALUE_NUM8, _VIRTUAL_NUM8},\
	{_TOUCH_BUTTON_VALUE_NUM9, _VIRTUAL_NUM9},\
	{_TOUCH_BUTTON_VALUE_NUMX, _VIRTUAL_NUMX},\
	{_TOUCH_BUTTON_VALUE_NUMH, _VIRTUAL_NUMH},\
};
#elif (TOUCH_KEYBOARD_TYPE == TOUCH_KEYBOARD_BS8112A3)
#define #define TOUCH_VALUE_TABLE \
{\
	{_TOUCH8112A3_VALUE_NUM0, _VIRTUAL_NUM0},\
	{_TOUCH8112A3_VALUE_NUM1, _VIRTUAL_NUM1},\
	{_TOUCH8112A3_VALUE_NUM2, _VIRTUAL_NUM2},\
	{_TOUCH8112A3_VALUE_NUM3, _VIRTUAL_NUM3},\
	{_TOUCH8112A3_VALUE_NUM4, _VIRTUAL_NUM4},\
	{_TOUCH8112A3_VALUE_NUM5, _VIRTUAL_NUM5},\
	{_TOUCH8112A3_VALUE_NUM6, _VIRTUAL_NUM6},\
	{_TOUCH8112A3_VALUE_NUM7, _VIRTUAL_NUM7},\
	{_TOUCH8112A3_VALUE_NUM8, _VIRTUAL_NUM8},\
	{_TOUCH8112A3_VALUE_NUM9, _VIRTUAL_NUM9},\
	{_TOUCH8112A3_VALUE_NUMX, _VIRTUAL_NUMX},\
	{_TOUCH8112A3_VALUE_NUMH, _VIRTUAL_NUMH},\
};

#else
#define TOUCH_VALUE_TABLE \
{
	{0,0},\
};
#endif

#endif

