
#ifndef __LIB_I2C_H__
#define __LIB_I2C_H__
#include <base_type.h>

H_BOOL lib_i2c_open(void);
H_U32 lib_i2c_write8(H_U8 slave_addr, H_U8 reg_addr, H_U8 reg_value);
H_U32 lib_i2c_write16(H_U8 slave_addr, H_U8 reg_addr, H_U16 reg_value);
H_U32 lib_i2c_write32(H_U8 slave_addr, H_U8 reg_addr, H_U32 reg_value);
H_U8 lib_i2c_read8(H_U8 slave_addr, H_U8 reg_addr);
H_U16 lib_i2c_read16(H_U8 slave_addr, H_U8 reg_addr);
H_U32 lib_i2c_read32(H_U8 slave_addr, H_U8 reg_addr);
H_U32 lib_i2c_read_ongoing(H_U8 slave_addr, H_U8 *data, H_U32 len);



#endif

