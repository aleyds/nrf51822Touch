#ifndef __TOUCH_TTY6952_H__
#define __TOUCH_TTY6952_H__

#define TTY6952_SLAVE_ADDR				(0x53<<1)		//芯片I2C从地址
#define TTY6952_WRITE_ADDR				(0xA6)			//芯片读数据地址
#define TTY6952_READ_ADDR				(0xA7)			//芯片读数据地址

/*******IICM***/
#define PC_LINK_MODE					(0)				//PC Link 模式
#define SLIDE_APP_MODE					(1)				//16键应用模式
#define IICM_BIT						(7)
/*******CT*Wheel application Mode******/
#define SETING_CMD_MODE					(0)				//应用设定
#define CUSTOM_CMD_MODE					(1)				//写入阀值设定
#define CT_BIT							(6)
/**********KOM************/
#define MULTIPLE_MODE					(0)				//多按键输出模式
#define SINGLE_MODE						(1)				//单一按键输出模式
#define KOM_BIT							(5)
/************AA基准值自动调整****************/
#define AA_DISABLE						(0)				//关闭
#define AA_ENABLE						(1)				//开启
#define AA_BIT							(4)
/************PSM省电模式********************/
#define PSM_DISABLE						(0)				//省电模式关闭 ,无按键4s后进入睡眠模式
#define PSM_ENABLE						(1)				//开启
#define PSM_BIT							(3)
/************ART*自动重置时间设定*************/
#define ART_DISABLE						(0)				//无重置时间
#define ART_SECOND_15					(0x1)			//按键没有变化时15s后自动重置
#define ART_SECOND_30					(0x2)			//30s
#define ART_SECOND_60					(0x3)			//60s
#define ART_SECOND_MASK					(0x3)
/************KAT*按键消抖时间*****************/
#define KAT_TIMES_1						(0x0)			//1Times
#define KAT_TIMES_2						(0x1)	
#define KAT_TIMES_3						(0x2)
#define KAT_TIMES_4						(0x3)
#define KAT_TIMES_5						(0x4)
#define KAT_TIMES_6						(0x5)
#define KAT_TIMES_7						(0x6)
#define KAT_TIMES_8						(0x7)
#define KAT_TIMES_MASK					(0x7)
/**********KEY Num****************************/
#define KEY_NUM_DISABLE					(0x00)			//未使能
#define KEY_NUM_1KEY					(0x01)			//1个按键
#define KEY_NUM_2KEYS					(0x02)			//两个按键
#define KEY_NUM_3KEYS					(0x03)
#define KEY_NUM_4KEYS					(0x04)
#define KEY_NUM_5KEYS					(0x05)
#define KEY_NUM_6KEYS					(0x06)
#define KEY_NUM_7KEYS					(0x07)
#define KEY_NUM_8KEYS					(0x08)
#define KEY_NUM_9KEYS					(0x09)
#define KEY_NUM_10KEYS					(0x0A)
#define KEY_NUM_11KEYS					(0x0B)
#define KEY_NUM_12KEYS					(0x0C)
#define KEY_NUM_13KEYS					(0x0D)
#define KEY_NUM_14KEYS					(0x0E)
#define KEY_NUM_15KEYS					(0x0F)
#define KEY_NUM_16KEYS					(0x10)
#define KEY_NUM_MASK					(0x1F)
#define KEY_NUM_BIT						(3)
/**********Key Off Num***多按键重置设定*********/
#define KEY_OFF_NUM_DISABLE				(0x0)			//两个按键同时重置
#define KEY_OFF_NUM_2KEY				(0x1)
#define KEY_OFF_NUM_3KEY				(0x2)
#define KEY_OFF_NUM_4KEY				(0x3)
#define KEY_OFF_NUM_5KEY				(0x4)
#define KEY_OFF_NUM_6KEY				(0x5)
#define KEY_OFF_NUM_7KEY				(0x6)
#define KEY_OFF_NUM_8KEY				(0x7)
#define KEY_OFF_NUM_9KEY				(0x8)
#define KEY_OFF_NUM_10KEY				(0x9)
#define KEY_OFF_NUM_11KEY				(0xA)
#define KEY_OFF_NUM_12KEY				(0xB)
#define KEY_OFF_NUM_13KEY				(0xC)
#define KEY_OFF_NUM_14KEY				(0xD)
#define KEY_OFF_NUM_15KEY				(0xE)
#define KEY_OFF_NUM_16KEY				(0xF)
#define KEY_OFF_NUM_MASK				(0xF)
#define KEY_OFF_NUM_BIT					(4)

/**************Custom threshold****************/
#define ITEM_TPx_SET					(0x0)				//按键阀值设定模式
#define ITEM_SLEEP_SET					(0x1)				//睡眠唤醒阀值设定
#define ITEM_MASK						(0x3)
#define ITEM_BIT						(4)
/************TP Num **************************/
#define TP_NUM0								(0x0)			//数据写入按键编号0
#define TP_NUM1								(0x1)
#define TP_NUM2								(0x2)
#define TP_NUM3								(0x3)
#define TP_NUM4								(0x4)
#define TP_NUM5								(0x5)
#define TP_NUM6								(0x6)
#define TP_NUM7								(0x7)
#define TP_NUM8								(0x8)
#define TP_NUM9								(0x9)
#define TP_NUM10							(0xA)
#define TP_NUM11							(0xB)
#define TP_NUM12							(0xC)
#define TP_NUM13							(0xD)
#define TP_NUM14							(0xE)
#define TP_NUM15							(0xF)
#define TP_NUM_MASK							(0xF)

#define TPX_SETTING_MASK					(0xF)
#define TPX_SETTING_L						(0)
#define TPX_SETTING_M						(4)
#define TPX_SETTING_H						(8)
#define TPX_SETTING_MIN						(0x001)  /*值越大灵敏度越低*/
#define TPX_SETTING_NORMAL					(20)
#define TPX_SETTING_MAX						(0xFFF)
#define TPX_SETTING_VALUE					(TPX_SETTING_NORMAL)

#define TPSLP_SETTING_MASK					(0xF)
#define TPSLP_SETTING_L						(0)
#define TPSLP_SETTING_M						(4)
#define TPSLP_SETTING_H						(8)
#define TPSLP_SETTING_MIN					(0x001)
#define TPSLP_SETTING_NORMAL				(0x002)
#define TPSLP_SETTING_MAX					(0xFFF)
#define TPSLP_SETTING_VALUE					(TPSLP_SETTING_NORMAL)


/*************Calibrate 系统校正标志****/
#define CALIBRATEING						(0x0)			//系统矫正中，按键值无效
#define CALIBRATE_FINISH					(0x1)			//矫正完成，按键值有效
#define CALIBRATE_BIT						(7)
/*************WSET  系统写入标志*********/
#define WSET_WRITE							(0x0)			//有数据写入
#define WSET_NOT_WRITE						(0x1)			//无数据写入，上电为1
#define WSET_BIT							(6)
/*************KEY PRESS*****************/
#define KEY_PRESS_RELEASE					(0x0)			//无按键按下
#define KEY_PRESS_DOWN						(0x1)			//有按键按下

/*键盘按键上报数据*/
enum touch6952_value{
	_TOUCH6952_VALUE_NULL,
	_TOUCH6952_VALUE_NUM0 = 0x0008,
	_TOUCH6952_VALUE_NUM1 = 0x0200,
	_TOUCH6952_VALUE_NUM2 = 0x1000,
	_TOUCH6952_VALUE_NUM3 = 0x4000,
	_TOUCH6952_VALUE_NUM4 = 0x0100,
	_TOUCH6952_VALUE_NUM5 = 0x2000,
	_TOUCH6952_VALUE_NUM6 = 0x8000,
	_TOUCH6952_VALUE_NUM7 = 0x0080,
	_TOUCH6952_VALUE_NUM8 = 0x0004,
	_TOUCH6952_VALUE_NUM9 = 0x0001,
	_TOUCH6952_VALUE_NUMX = 0x0040,//按键*
	_TOUCH6952_VALUE_NUMH = 0x0002,//按键#
};

H_U32 m2mkey_tty6952_open(void);
H_U32 m2mkey_tty6952_obtain(void);

#endif

