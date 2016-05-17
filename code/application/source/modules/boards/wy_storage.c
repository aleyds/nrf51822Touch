#include "wy_tools.h"
#include "wy_uart.h"
#include "wy_storage.h"

H_U32 _StorageOpen(void)
{
	H_U32 err_code; 
	// Initialize persistent storage module. 
	err_code = pstorage_init(); 
	APP_ERROR_CHECK(err_code);
	return err_code;
}



H_U32  _StorageRegister(pstorage_handle_t *flash_handle, H_U32 block_size,H_U32 block_count,stroage_callback cb)
{
	H_U32 err_code = 0;
	pstorage_module_param_t PST_param; 
	
	wy_memset(&PST_param, 0 ,sizeof(pstorage_module_param_t));
	PST_param.block_size = block_size; 
	PST_param.block_count = block_count; 
	PST_param.cb = cb; 
	err_code = pstorage_register(&PST_param,flash_handle); 
	APP_ERROR_CHECK(err_code); 
	return err_code;

}

H_U32 _StorageErase(pstorage_handle_t *flash_handle,H_U32 size)
{
	WYPrintf(MODULE_DEBUG_STORAGE,LEVEL_DEBUG,"erasing...");
	H_U32 err_code = 0; 
	err_code = pstorage_clear(flash_handle,size); 
	wy_tools_op()->_delay_ms(100);
	return err_code;
	//APP_ERROR_CHECK(err_code); 

}

H_U32 _StorageWrite(pstorage_handle_t *flash_handle,H_U8 *data,H_U32 size,H_U32 offset,H_U16 user_block_size)
{
	H_U32 err_code = 0; 
	H_U32 correct_offset = 0;
	pstorage_handle_t redirect_handle;

	wy_memset(&redirect_handle, 0, sizeof(pstorage_handle_t));
	correct_offset = (offset * user_block_size);
	err_code= pstorage_block_identifier_get(flash_handle, 0, &redirect_handle); 
	APP_ERROR_CHECK(err_code); 
	WYPrintf(MODULE_DEBUG_STORAGE,LEVEL_DEBUG,"before write2 for module:%d",flash_handle->module_id);
	err_code = pstorage_store(&redirect_handle,(H_U8 *)data,size,correct_offset); 
	wy_tools_op()->_delay_us(100);
	return err_code;
	//APP_ERROR_CHECK(err_code); 

}

H_U32 _StorageRead(pstorage_handle_t *flash_handle,H_U32 size,H_U32 offset,H_U8 *date,H_U16 user_block_size)
{
	H_U32 err_code = 0; 
	H_U32 correct_offset = 0;
	pstorage_handle_t redirect_handle;

	wy_memset(&redirect_handle, 0, sizeof(pstorage_handle_t));
	correct_offset = (offset * user_block_size);
	err_code = pstorage_block_identifier_get(flash_handle,0,&redirect_handle); 
	APP_ERROR_CHECK(err_code); 
	err_code = pstorage_load(date, &redirect_handle,size,correct_offset); 
	wy_tools_op()->_delay_us(100);
	return err_code;

}

H_U32 _StorageUpdata(device_storage_gather_t *device_info)
{
	H_U32 retcode;
	WYPrintf(MODULE_DEBUG_STORAGE,LEVEL_DEBUG,"in __storage_device_updata");
	CHECK_PARAMETER(device_info, H_NULL, H_FAULTER);
	//H_U8 * buffer = H_NULL;
	//buffer = (H_U8 *)wy_malloc(STORAGE_BLOCK_LEN);
	//if( buffer == H_NULL)
	//{
	//	WYPrintf(MODULE_DEBUG_STORAGE,LEVEL_DEBUG,"malloc fail");
	//	return H_FAULTER;
	//}
	static H_U8 storage_buffer[STORAGE_BLOCK_LEN] = {0};
	H_U32 offset = 0;
	H_U32 magic_data = 0;

	wy_memset(storage_buffer,0xFF,STORAGE_BLOCK_LEN);
	//WYPrintf(MODULE_DEBUG_STORAGE,LEVEL_DEBUG,"reading entire block...");
	if(_StorageRead(device_info->handler, STORAGE_BLOCK_LEN,0,storage_buffer,device_info->user_block_size) != H_SUCCESS)
	{
		WYPrintf(MODULE_DEBUG_STORAGE,LEVEL_DEBUG,"read fail");
		//wy_free(buffer);
		//buffer = H_NULL;
		return H_FAULTER;
	}
	//WYPrintf_array(storage_buffer, STORAGE_BLOCK_LEN);
	wy_tools_op()->_delay_ms(10);
	offset = device_info->id * device_info->user_block_size ;
	WYPrintf(MODULE_DEBUG_STORAGE,LEVEL_DEBUG,"offset will be:%d",offset);
	magic_data = decode32(storage_buffer, offset);
	if(magic_data == device_info->magic_data || magic_data == 0) // check whether this block has been written before and is written by correct module
	{                                                          //workaround
		WYPrintf(MODULE_DEBUG_STORAGE,LEVEL_DEBUG,"magic1");
		if(device_info->magic_clear)//delete a record
		{
			WYPrintf(MODULE_DEBUG_STORAGE,LEVEL_DEBUG,"magic2");
			wy_memset(&(storage_buffer[offset]), 0xff, device_info->hand_len);
		}
		wy_memcpy(&(storage_buffer[offset + device_info->hand_len]), device_info->data, device_info->data_len);
	}
	else  //this block has not been written before 
	{
		if(!device_info->magic_clear)//add a record 
		{
			encode32(device_info->magic_data, storage_buffer, offset);
			wy_memcpy(&(storage_buffer[offset + device_info->hand_len]), device_info->data, device_info->data_len);
			
			WYPrintf(MODULE_DEBUG_STORAGE,LEVEL_DEBUG,"magic3:%x ",device_info->magic_data);
			
		}
	}
	retcode = _StorageErase(device_info->handler,STORAGE_BLOCK_LEN);
	WYPrintf(MODULE_DEBUG_STORAGE,LEVEL_DEBUG,"erasing module:%d block :%d",device_info->handler->module_id,device_info->handler->block_id);
	WYPrintf(MODULE_DEBUG_STORAGE,LEVEL_DEBUG,"ret of erase:%d",retcode);
	if(retcode  != H_SUCCESS)
		
	{
		WYPrintf(MODULE_DEBUG_STORAGE,LEVEL_DEBUG,"erase fail");
		//wy_free(buffer);
		//buffer = H_NULL;
		return H_FAULTER;
	}
	//wy_tools_op()->_delay_ms(100);
	//WYPrintf(MODULE_DEBUG_STORAGE,LEVEL_DEBUG,"before write1 for module:%d",device_info->handler->module_id);
	retcode = _StorageWrite(device_info->handler,storage_buffer,STORAGE_BLOCK_LEN, 0,device_info->user_block_size);
	WYPrintf(MODULE_DEBUG_STORAGE,LEVEL_DEBUG,"ret of write:%d  block size:%d",retcode,device_info->user_block_size);
	if(retcode != H_SUCCESS)
	{
		WYPrintf(MODULE_DEBUG_STORAGE,LEVEL_DEBUG,"rewrite fail");
		//wy_free(buffer);
		//buffer = H_NULL;
		return H_FAULTER;
	}
	wy_tools_op()->_delay_ms(100);
	//wy_free(buffer);
	//buffer = H_NULL;
	return H_SUCCESS;
}


H_U32 _StorageCount(void)
{
	H_U32 count = 0;  
//	H_U32 retval = 0;
	// Request clearing of blocks 
	pstorage_access_status_get(&count); 
	return count;
}

