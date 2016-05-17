#include "base_type.h"
#include "lib_i2c.h"
#include "wy_uart.h"
#include "wy_tools.h"
#include "wy_touch_tty6952.h"

#define TTY6952_DEBUG		"TTY6952"

static H_U32 __tty6952_setting_cmd(void)
{
	H_U8 Byte1 = ((SLIDE_APP_MODE<<IICM_BIT) | (SETING_CMD_MODE<<CT_BIT) | (SINGLE_MODE<<KOM_BIT)\
				   |(AA_DISABLE<<AA_BIT)|(PSM_ENABLE<<PSM_BIT)|(ART_DISABLE&ART_SECOND_MASK))&0xFF;
	H_U8 Byte2 = ((KAT_TIMES_3&KAT_TIMES_MASK)|((KEY_NUM_16KEYS&KEY_NUM_MASK)<<KEY_NUM_BIT))&0xFF;
	H_U8 Byte3 = ((KEY_OFF_NUM_DISABLE&KEY_OFF_NUM_MASK)<<KEY_OFF_NUM_BIT)&0xFF;
	H_U32 Setting = (Byte1 | ((Byte2<<8)&0xFF00) | ((Byte3 << 16)&0xFF0000));
	return Setting;
}

static H_U32 __tty6952_custom_cmd(H_U8 Item, H_U8 tp_num, H_U16 value)
{
	H_U8 Byte1 = ((SLIDE_APP_MODE<<IICM_BIT) | (CUSTOM_CMD_MODE<<CT_BIT) | ((Item&ITEM_MASK)<<ITEM_BIT)\
					| (tp_num&TP_NUM_MASK));
	
	H_U8 Byte2 = (value&TPX_SETTING_MASK)|(((value >> TPX_SETTING_M)&TPX_SETTING_MASK)<<TPX_SETTING_M);
	H_U8 Byte3 = ((value >> TPX_SETTING_H)&TPX_SETTING_MASK);
	H_U32 Setting = (Byte1 | ((Byte2<<8)&0xFF00) | ((Byte3 << 16)&0xFF0000));
	return Setting;
}

static H_U32 __tty6952_config(void)
{
	H_U32 setting = __tty6952_setting_cmd();
	H_U32 custom = 0;
	lib_i2c_write16(TTY6952_SLAVE_ADDR, (H_U8)(setting&0xFF),(H_U16)((setting>>8)&0xFFFF));
	
	for(H_U32 i = 0; i <= TP_NUM15 ; i++)
	{
		custom = __tty6952_custom_cmd(ITEM_TPx_SET, i, TPX_SETTING_VALUE);
		lib_i2c_write16(TTY6952_SLAVE_ADDR, (H_U8)(custom&0xFF),(H_U16)((custom>>8)&0xFFFF));

		custom = __tty6952_custom_cmd(ITEM_SLEEP_SET, i, TPSLP_SETTING_VALUE);
		lib_i2c_write16(TTY6952_SLAVE_ADDR, (H_U8)(custom&0xFF),(H_U16)((custom>>8)&0xFFFF));
	}
	
	
	//H_U32 custom = __tty6952_custom_cmd(TP_NUM0);
	
	return H_SUCCESS;
}

H_U32 m2mkey_tty6952_open(void)
{
	return __tty6952_config();
}

H_U32 m2mkey_tty6952_obtain(void)
{
	H_U32 data  = lib_i2c_read32(TTY6952_SLAVE_ADDR, TTY6952_READ_ADDR);
	//data = byte32_turn(data);
	H_U8 Byte1 = data&0xFF;
	//__tty6952_config();
	if(((Byte1>>CALIBRATE_BIT)&0x1) && (!((Byte1>>WSET_BIT)&0x1)))
	{
		return ((data>>8)&0xFFFF);
	}
	
	return 0;
}



