#ifndef __SYS_CONFIG_H__
#define __SYS_CONFIG_H__



/******************Common config***********/
#define SDK_VERSION				"7.0"
#define SW_VERSION				"1.0"
#define HW_VERSION				"1.0"
#define SUPPLIER				"m2mkey"

/******************LIB Config**************/
#define SPI_MASTER_0_ENABLE
#undef SPI_MASTER_1_ENABLE
#define LIB_I2C_ENABLE
#define LIB_SPI_ENABLE
#undef LIB_RTC_ENABLE

#define USE_MAIN_TASK

/******************Module config**********/
#define MODULE_STORAGE_ENABLE				
#define MODULE_TIMER_ENABLE					
#define MODULE_ADV_ENABLE					
#define MODULE_UART_ENABLE					
#define MODULE_TOOLS_ENABLE					
#define MODULE_BLE_ENABLE					
#define MODULE_NEAR_CONTROL_ENABLE		
#define MODULE_SYSTEM_ENABLE				
#define MODULE_ALARM_ENABLE					
#define MODULE_BATTERY_ENABLE				
#define MODULE_LED_ENABLE
#define MODULE_DFU_ENABLE

/************************Finger Module****************/


/************************Keyboard*********************/
#define TOUCH_NULL									(0xFFFF)
#define MODULE_TOUCH_KEYBOARD_ENABLE								/*密码模块*/
#define TOUCH_KEYBOARD_NORMAL						(0x0)			/*触摸式I2C扫描按键*/
#define TOUCH_KEYBOARD_TTY6952						(0x01)			/*触摸式I2C触发按键*/
#define TOUCH_KEYBOARD_BUTTON						(0x02)			/*实体扫描按键*/
#define TOUCH_KEYBOARD_BS8112A3						(0x03)
#define TOUCH_KEYBOARD_TYPE							TOUCH_KEYBOARD_TTY6952
#define TOUCH_LED_GUIDE								(0x0)			/*导光板LED*/
#define TOUCH_LED_INDEPENDENT						(0x1)			/*独立LED显示*/
#define TOUCH_LED_TYPE								(TOUCH_LED_INDEPENDENT)


#define WDG_WAIT_TIME			(180) //60 sec

/****************************调试信息************************************/
#define DEBUG_SWITCH
#define LEVEL_DEBUG 			(1)
#define LEVEL_WARNING			(2)
#define LEVEL_ERROR				(3)
#define LEVEL_INFO				(4)
#define DEBUG_LEVEL_ENABLE		(0) /*0:全开  其他大于该数数值才为开启*/


#define NONE         			"\033[m" 
#define RED          			"\033[0;32;31m" 
#define LIGHT_RED    			"\033[1;31m" 
#define GREEN        			"\033[0;32;32m" 
#define LIGHT_GREEN  			"\033[1;32m" 
#define BLUE         			"\033[0;32;34m" 
#define LIGHT_BLUE   			"\033[1;34m" 
#define DARY_GRAY    			"\033[1;30m" 
#define CYAN         			"\033[0;36m" 
#define LIGHT_CYAN   			"\033[1;36m" 
#define PURPLE       			"\033[0;35m" 
#define LIGHT_PURPLE 			"\033[1;35m" 
#define BROWN        			"\033[0;33m" 
#define YELLOW       			"\033[1;33m" 
#define LIGHT_GRAY   			"\033[0;37m" 
#define WHITE        			"\033[1;37m"


#if defined(DEBUG_SWITCH)&&defined(MODULE_UART_ENABLE)

#define WYPrintf(moulde,level,fmt,args...)\
do{\
	if(level >= DEBUG_LEVEL_ENABLE)\
	{\
		if(level == LEVEL_DEBUG)\
		{\
			_UartPrintf("["moulde"][%d]"fmt"\n\r",level,##args);\
		}\
		else if(level == LEVEL_WARNING)\
		{\
			_UartPrintf(BLUE"["moulde"][%d]"fmt"\n\r"NONE,level,##args);\
		}\
		else if(level == LEVEL_ERROR)\
		{\
			_UartPrintf(RED"["moulde"][%d]"fmt"\n\r"NONE,level,##args);\
		}\
		else \
		{\
			_UartPrintf(GREEN"["moulde"][%d]"fmt"\n\r"NONE,level,##args);\
		}\
	}\
}while(0);



#define WYPrintf_array(array,len)\
do{\
	H_U32 i;\
	for(i = 0; i < len; i++)\
	{\
		if(i%16 == 0)\
		{\
			_UartPrintf("\n\r");\
		}\
		_UartPrintf("%x ",array[i]);\
	}\
	_UartPrintf("\n\r len:%d \n\r",i);\
}while(0);

#define WYPrintf_array_str(tag , array,len)\
do{\
	H_U8 array_str[(len<<1) + 1] = { 0 };\
	wy_tools_op()->_to_str(array, len , array_str, sizeof(array_str));\
	WYPrintf("ARRAY STR", LEVEL_DEBUG, tag" array str:%s",array_str);\
}while(0);

#else
#define WYPrintf(moulde,level,fmt,args...)			//do{}while(0);
#define WYPrintf_array(array,len)					//do{}while(0);
#define WYPrintf_array_str(tag , array,len)		//do{}while(0);

#endif


#endif

