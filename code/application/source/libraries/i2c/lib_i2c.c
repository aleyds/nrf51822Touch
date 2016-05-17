#include "config.h"
#ifdef LIB_I2C_ENABLE
#include <wy_uart.h>
#include <twi_master.h>
#include <lib_i2c.h>

#define USER_HW_I2C

H_BOOL lib_i2c_open(void)
{
	return twi_master_init();
}

H_U32 lib_i2c_write8(H_U8 slave_addr, H_U8 reg_addr, H_U8 reg_value)
{
#ifdef USER_HW_I2C
		H_U8 buffer[2] = { 0 };
		buffer[0] = (reg_addr & 0xff);
		buffer[1] = ((reg_value) & 0xff);
		if(twi_master_transfer((slave_addr | TWI_WRITE_BIT),(H_U8 *)buffer,sizeof(buffer), TWI_ISSUE_STOP))
		{
			return H_SUCCESS;
		}
#else
		if(twi_master_transfer_ext(slave_addr, reg_addr,(H_U8 *)&reg_value,sizeof(H_U8)))
		{
			return H_SUCCESS;
		}
#endif

	return H_FAULTER;
}

H_U32 lib_i2c_write16(H_U8 slave_addr, H_U8 reg_addr, H_U16 reg_value)
{
#ifdef USER_HW_I2C
	H_U8 buffer[3] = { 0 };
	buffer[0] = (reg_addr & 0xff);
	buffer[1] = (reg_value & 0xff);
	buffer[2] = ((reg_value >> 8) & 0xff);
	if(twi_master_transfer((slave_addr | TWI_WRITE_BIT),(H_U8 *)buffer, sizeof(buffer), TWI_ISSUE_STOP))
	{
		return H_SUCCESS;
	}
#else
	H_U8 buffer[2] = { 0 };
	buffer[0] = (reg_value & 0xff);
	buffer[1] = ((reg_value >> 8) & 0xff);
	if(twi_master_transfer_ext(slave_addr, reg_addr,(H_U8 *)buffer,sizeof(buffer)))
	{
		return H_SUCCESS;
	}

#endif
	return H_FAULTER;
}


H_U32 lib_i2c_write32(H_U8 slave_addr, H_U8 reg_addr, H_U32 reg_value)
{
#ifdef USER_HW_I2C
	H_U8 buffer[5] = { 0 };
	buffer[0] = (reg_addr & 0xff);
	buffer[1] = (reg_value & 0xff);
	buffer[2] = ((reg_value >> 8) & 0xff);
	buffer[3] = ((reg_value >> 16) & 0xff);
	buffer[4] = ((reg_value >> 24) & 0xff);
	if(twi_master_transfer((slave_addr | TWI_WRITE_BIT),(H_U8 *)buffer, sizeof(buffer), TWI_ISSUE_STOP))
	{
		return H_SUCCESS;
	}
#else
	H_U8 buffer[4] = { 0 };
	buffer[0] = (reg_value & 0xff);
	buffer[1] = ((reg_value >> 8) & 0xff);
	buffer[2] = ((reg_value >> 16) & 0xff);
	buffer[3] = ((reg_value >> 24) & 0xff);
	if(twi_master_transfer_ext(slave_addr, reg_addr,(H_U8 *)buffer,sizeof(buffer)))
	{
		return H_SUCCESS;
	}
#endif
	return H_FAULTER;
}


H_U32 lib_i2c_read_ongoing(H_U8 slave_addr, H_U8 *data, H_U32 len)
{
	if(twi_master_transfer((slave_addr | TWI_READ_BIT), (H_U8 *)data, len, TWI_ISSUE_STOP))
	{
		return H_SUCCESS;
	}
	return H_FAULTER;
}


H_U8 lib_i2c_read8(H_U8 slave_addr, H_U8 reg_addr)
{
	H_U8 read_data = 0;
	
	if(twi_master_transfer(slave_addr, (H_U8 *)&reg_addr, sizeof(H_U8), TWI_DONT_ISSUE_STOP))
	{
		if(twi_master_transfer((slave_addr | TWI_READ_BIT), (H_U8 *)&read_data, sizeof(H_U8), TWI_ISSUE_STOP))
		{
			return read_data;
		}
	}
	return 0;
}

H_U16 lib_i2c_read16(H_U8 slave_addr, H_U8 reg_addr)
{
	H_U8 buffer[2] = { 0 };
	if(twi_master_transfer(slave_addr, (H_U8 *)&reg_addr, sizeof(H_U8), TWI_DONT_ISSUE_STOP))
	{
		if(twi_master_transfer((slave_addr | TWI_READ_BIT), (H_U8 *)buffer, sizeof(H_U16), TWI_ISSUE_STOP))
		{
			return (buffer[0] | (buffer[1] << 8));
		}
	}
	return 0;
}

H_U32 lib_i2c_read32(H_U8 slave_addr, H_U8 reg_addr)
{
	H_U8 buffer[4] = { 0 };
	if(twi_master_transfer(slave_addr, (H_U8 *)&reg_addr, sizeof(H_U8), TWI_DONT_ISSUE_STOP))
	{
		if(twi_master_transfer((slave_addr | TWI_READ_BIT), (H_U8 *)buffer, sizeof(H_U32), TWI_ISSUE_STOP))
		{
			return (buffer[0] | (buffer[1] << 8) | (buffer[2] << 16) | (buffer[3] << 24));
		}
	}
	return 0;
}

#endif

