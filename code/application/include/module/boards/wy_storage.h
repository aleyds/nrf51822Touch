/******************************************************************************

*******************************************************************************/
#ifndef __HS_STORAGE_H__
#define __HS_STORAGE_H__
#include <base_type.h>
#include <pstorage.h>
#include <app_error.h>

#define STO_DEBUG 1
#ifdef STO_DEBUG
#endif
#define MODULE_DEBUG_STORAGE   "STORAGE"
#define STORAGE_BLOCK_LEN (1024)  /*Flash 存储块的大小*/
//#define DATA_BLOCK_LEN (24) /*Flash 用户存储数据块大小*/

typedef void (*stroage_callback)(pstorage_handle_t * handle,H_U8 op_code, 
									H_U32 result, H_U8 * p_data, H_U32 data_len);

typedef struct device_storage_gather{
	pstorage_handle_t *handler;
	H_U32 id;
	H_U32 hand_len;
	H_U8 *data;
	H_U16 data_len;
	H_U16 user_block_size;
	H_U16 storage_counter;
	H_BOOL magic_clear;
	H_U32 magic_data;
}device_storage_gather_t;


H_U32 _StorageOpen(void);
H_U32 _StorageRegister(pstorage_handle_t *flash_handle, H_U32 block_size,H_U32 block_count,stroage_callback cb);
H_U32 _StorageErase(pstorage_handle_t *flash_handle,H_U32 size);
H_U32 _StorageWrite(pstorage_handle_t *flash_handle,H_U8 *data,H_U32 size,H_U32 offset,H_U16 user_block_size);
H_U32 _StorageRead(pstorage_handle_t *flash_handle,H_U32 size,H_U32 offset,H_U8 *date,H_U16 user_block_size);
H_U32 _StorageUpdata(device_storage_gather_t *device_info);
H_U32 _StorageCount(void);



#endif

