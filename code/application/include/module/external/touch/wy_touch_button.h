#ifndef __TOUCH_BUTTON_H__
#define __TOUCH_BUTTON_H__

enum TouchButtonPort{
	_PORT_KEY1 = KEYBOARD_KEY1,
	_PORT_KEY2 = KEYBOARD_KEY2,
	_PORT_KEY3 = KEYBOARD_KEY3,
	_PORT_KEY4 = KEYBOARD_KEY4,
	_PORT_KEY5 = KEYBOARD_KEY5,
	_PORT_KEY6 = KEYBOARD_KEY6,
	_PORT_KEY7 = KEYBOARD_KEY7,
};
#define KEY1_BIT			(0x0)
#define KEY2_BIT			(0x1)
#define KEY3_BIT			(0x2)
#define KEY4_BIT			(0x3)
#define KEY5_BIT			(0x4)
#define KEY6_BIT			(0x5)
#define KEY7_BIT			(0x6)
#define DATA_MASK			(0x01)
#define DEFAULT_SETTING		(0xFF)

enum VHType{
	_TYPE_VH_NULL,
	_TYPE_VH_TYPE0,
	_TYPE_VH_TYPE1,
	_TYPE_VH_TYPE2,
	_TYPE_VH_TYPE3,
	
};

/*键盘按键上报数据*/
enum touch_button_value{
	_TOUCH_BUTTON_VALUE_NULL,
	_TOUCH_BUTTON_VALUE_NUM0 = 0x000A,
	_TOUCH_BUTTON_VALUE_NUM1 = 0x0041,
	_TOUCH_BUTTON_VALUE_NUM2 = 0x0042,
	_TOUCH_BUTTON_VALUE_NUM3 = 0x0044,
	_TOUCH_BUTTON_VALUE_NUM4 = 0x0021,
	_TOUCH_BUTTON_VALUE_NUM5 = 0x0022,
	_TOUCH_BUTTON_VALUE_NUM6 = 0x0024,
	_TOUCH_BUTTON_VALUE_NUM7 = 0x0011,
	_TOUCH_BUTTON_VALUE_NUM8 = 0x0012,
	_TOUCH_BUTTON_VALUE_NUM9 = 0x0014,
	_TOUCH_BUTTON_VALUE_NUMX = 0x0009,//按键*
	_TOUCH_BUTTON_VALUE_NUMH = 0x000C,//按键#
};

H_U32 m2mkey_button_obtain(void);

#endif

