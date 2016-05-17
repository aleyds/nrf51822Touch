/* Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

#include <string.h>
#include "ble_nus.h"
#include "nordic_common.h"
#include "ble_srv_common.h"


/**@brief     Function for handling the @ref BLE_GAP_EVT_CONNECTED event from the S110 SoftDevice.
 *
 * @param[in] p_nus     Nordic UART Service structure.
 * @param[in] p_ble_evt Pointer to the event received from BLE stack.
 */
static void on_connect(ble_nus_t * p_nus, ble_evt_t * p_ble_evt)
{
    p_nus->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
}


/**@brief     Function for handling the @ref BLE_GAP_EVT_DISCONNECTED event from the S110
 *            SoftDevice.
 *
 * @param[in] p_nus     Nordic UART Service structure.
 * @param[in] p_ble_evt Pointer to the event received from BLE stack.
 */
static void on_disconnect(ble_nus_t * p_nus, ble_evt_t * p_ble_evt)
{
    UNUSED_PARAMETER(p_ble_evt);
    p_nus->conn_handle = BLE_CONN_HANDLE_INVALID;
}


/**@brief     Function for handling the @ref BLE_GATTS_EVT_WRITE event from the S110 SoftDevice.
 *
 * @param[in] p_dfu     Nordic UART Service structure.
 * @param[in] p_ble_evt Pointer to the event received from BLE stack.
 */
static void on_write(ble_nus_t * p_nus, ble_evt_t * p_ble_evt)
{
    ble_gatts_evt_write_t * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;
    
    if ((p_evt_write->handle == p_nus->status_handles.cccd_handle)&&
        (p_evt_write->len == 2))
    {
		if (ble_srv_is_notification_enabled(p_evt_write->data))
		{
			p_nus->is_notification_enabled = true;
		}
		else
		{
			p_nus->is_notification_enabled = false;
		}
    }
   else if ((p_evt_write->handle == p_nus->nonce_handles.cccd_handle)&&
        	(p_evt_write->len == 2))
    {
		if (ble_srv_is_notification_enabled(p_evt_write->data))
		{
			p_nus->is_notification_enabled = true;
		}
		else
		{
			p_nus->is_notification_enabled = false;
		}
    }
    else if ((p_evt_write->handle == p_nus->command_handles.value_handle)&&
             (p_nus->data_handler != NULL))
    {
		volatile data_type mode = _TYPE_COMMAND;
		p_nus->data_handler(p_nus, p_evt_write->data,(uint16_t)mode,0);
    }
	else if ((p_evt_write->handle == p_nus->nonce_handles.value_handle)&&
             (p_nus->data_handler != NULL))
    {
		volatile data_type mode = _TYPE_NONCE;
		p_nus->data_handler(p_nus, p_evt_write->data,(uint16_t)mode,0);
    }
	else if ((p_evt_write->handle == p_nus->start_handles.value_handle)&&
             (p_nus->data_handler != NULL))
    {
		volatile data_type mode = _TYPE_SESSION;
		p_nus->data_handler(p_nus, p_evt_write->data,(uint16_t)mode,0);
    }
    else
    {
        // Do Nothing. This event is not relevant to this service.
    }
}


/**@brief       Function for adding RX characteristic.
 *
 * @param[in]   p_nus        Nordic UART Service structure.
 * @param[in]   p_nus_init   Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t inf_alarm_status_char_add(ble_nus_t * p_nus, const ble_nus_init_t * p_nus_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;
    
    memset(&cccd_md, 0, sizeof(cccd_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

    cccd_md.vloc = BLE_GATTS_VLOC_STACK;
    
    memset(&char_md, 0, sizeof(char_md));
    
    char_md.char_props.notify   = 1;
	  char_md.char_props.read     = 1;
	  char_md.char_props.indicate = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;
    
    ble_uuid.type             = p_nus->uuid_type;
    ble_uuid.uuid             = BLE_UUID_INF_ALERM_STATUS_CHARACTERISTIC;
    
    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    
    attr_md.vloc              = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth           = 0;
    attr_md.wr_auth           = 0;
    attr_md.vlen              = 1;
    
    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = sizeof(uint8_t);
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = BLE_NUS_MAX_INF_ALERM_STATUS_CHAR_LEN;
    attr_char_value.p_value   = 0;
		
    return sd_ble_gatts_characteristic_add(p_nus->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_nus->status_handles);

}


/**@brief       Function for adding TX characteristic.
 *
 * @param[in]   p_nus        Nordic UART Service structure.
 * @param[in]   p_nus_init   Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t inf_alarm_command_char_add(ble_nus_t * p_nus, const ble_nus_init_t * p_nus_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;
    
    memset(&char_md, 0, sizeof(char_md));
    
    char_md.char_props.write            = 1;
    char_md.char_props.write_wo_resp    = 1;
    char_md.p_char_user_desc            = NULL;
    char_md.p_char_pf                   = NULL;
    char_md.p_user_desc_md              = NULL;
    char_md.p_cccd_md                   = NULL;
    char_md.p_sccd_md                   = NULL;
    
    ble_uuid.type                       = p_nus->uuid_type;
    ble_uuid.uuid                       = BLE_UUID_INF_ALERM_COMMAND_CHARACTERISTIC;
    
    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    
    attr_md.vloc                        = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth                     = 0;
    attr_md.wr_auth                     = 0;
    attr_md.vlen                        = 1;
    
    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid              = &ble_uuid;
    attr_char_value.p_attr_md           = &attr_md;
    attr_char_value.init_len            = 1;
    attr_char_value.init_offs           = 0;
    attr_char_value.max_len             = BLE_NUS_MAX_INF_ALERM_COMMAND_CHAR_LEN;
    
    return sd_ble_gatts_characteristic_add(p_nus->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_nus->command_handles);
}

/**@brief       Function for adding RX characteristic.
 *
 * @param[in]   p_nus        Nordic UART Service structure.
 * @param[in]   p_nus_init   Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t nonce_char_add(ble_nus_t * p_nus, const ble_nus_init_t * p_nus_init,uint8_t *data)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_md_t cccd_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;
    
    memset(&cccd_md, 0, sizeof(cccd_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);

    cccd_md.vloc = BLE_GATTS_VLOC_STACK;
    
    memset(&char_md, 0, sizeof(char_md));
    
    char_md.char_props.notify = 1;
	char_md.char_props.read   = 1;
	char_md.char_props.write  = 1;
    char_md.p_char_user_desc  = NULL;
    char_md.p_char_pf         = NULL;
    char_md.p_user_desc_md    = NULL;
    char_md.p_cccd_md         = &cccd_md;
    char_md.p_sccd_md         = NULL;
    
    ble_uuid.type             = p_nus->uuid_type;
    ble_uuid.uuid             = BLE_UUID_NONCE_CHARACTERISTIC;
    
    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    
    attr_md.vloc              = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth           = 0;
    attr_md.wr_auth           = 0;
    attr_md.vlen              = 1;
    
    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid    = &ble_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.init_len  = 16;
    attr_char_value.init_offs = 0;
    attr_char_value.max_len   = BLE_NUS_MAX_INF_ALERM_STATUS_CHAR_LEN;
    attr_char_value.p_value   = data; 
		
    return sd_ble_gatts_characteristic_add(p_nus->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_nus->nonce_handles);
    

}


/**@brief       Function for adding TX characteristic.
 *
 * @param[in]   p_nus        Nordic UART Service structure.
 * @param[in]   p_nus_init   Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on success, otherwise an error code.
 */
static uint32_t start_session_char_add(ble_nus_t * p_nus, const ble_nus_init_t * p_nus_init)
{
    ble_gatts_char_md_t char_md;
    ble_gatts_attr_t    attr_char_value;
    ble_uuid_t          ble_uuid;
    ble_gatts_attr_md_t attr_md;
    
    memset(&char_md, 0, sizeof(char_md));
    
    char_md.char_props.write            = 1;
    char_md.char_props.write_wo_resp    = 1;
    char_md.p_char_user_desc            = NULL;
    char_md.p_char_pf                   = NULL;
    char_md.p_user_desc_md              = NULL;
    char_md.p_cccd_md                   = NULL;
    char_md.p_sccd_md                   = NULL;
    
    ble_uuid.type                       = p_nus->uuid_type;
    ble_uuid.uuid                       = BLE_UUID_START_SESSION_CHARACTERISTIC;
    
    memset(&attr_md, 0, sizeof(attr_md));

    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    
    attr_md.vloc                        = BLE_GATTS_VLOC_STACK;
    attr_md.rd_auth                     = 0;
    attr_md.wr_auth                     = 0;
    attr_md.vlen                        = 1;
    
    memset(&attr_char_value, 0, sizeof(attr_char_value));

    attr_char_value.p_uuid              = &ble_uuid;
    attr_char_value.p_attr_md           = &attr_md;
    attr_char_value.init_len            = 1;
    attr_char_value.init_offs           = 0;
    attr_char_value.max_len             = BLE_NUS_MAX_INF_ALERM_COMMAND_CHAR_LEN;
    
    return sd_ble_gatts_characteristic_add(p_nus->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_nus->start_handles);
}


void ble_nus_on_ble_evt(ble_nus_t * p_nus, ble_evt_t * p_ble_evt)
{
    if ((p_nus == NULL) || (p_ble_evt == NULL))
    {
        return;
    }

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            on_connect(p_nus, p_ble_evt);
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            on_disconnect(p_nus, p_ble_evt);
            break;

        case BLE_GATTS_EVT_WRITE:
            on_write(p_nus, p_ble_evt);
            break;

        default:
            // No implementation needed.
            break;
    }
}


uint32_t ble_nus_init(ble_nus_t * p_nus, const ble_nus_init_t * p_nus_init,uint8_t *nance_data)
{
    uint32_t        err_code;
    ble_uuid_t      ble_uuid;
    ble_uuid128_t   nus_base_uuid = {0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80,
                                     0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    if ((p_nus == NULL) || (p_nus_init == NULL))
    {
        return NRF_ERROR_NULL;
    }
    
    // Initialize service structure.
    p_nus->conn_handle              = BLE_CONN_HANDLE_INVALID;
    p_nus->data_handler             = p_nus_init->data_handler;
    p_nus->is_notification_enabled  = false;
    

    /**@snippet [Adding proprietary Service to S110 SoftDevice] */

    // Add custom base UUID.
    err_code = sd_ble_uuid_vs_add(&nus_base_uuid, &p_nus->uuid_type);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    ble_uuid.type = p_nus->uuid_type;
    ble_uuid.uuid = BLE_UUID_INF_ALERM_SERVICE;

    // Add service.
    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                                        &ble_uuid,
                                        &p_nus->service_handle);
    /**@snippet [Adding proprietary Service to S110 SoftDevice] */
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }
    
		
	 // Add command Characteristic.
   err_code = inf_alarm_command_char_add(p_nus, p_nus_init);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }
		
    // Add status Characteristic.
    err_code = inf_alarm_status_char_add(p_nus, p_nus_init);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }
		
		// Add nonce Characteristic.
    err_code = nonce_char_add(p_nus, p_nus_init,nance_data);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }

    // Add start_session Characteristic.
    err_code = start_session_char_add(p_nus, p_nus_init);
    if (err_code != NRF_SUCCESS)
    {
        return err_code;
    }
    
    return NRF_SUCCESS;
}


uint32_t ble_nus_send_status(ble_nus_t * p_nus, uint8_t * string, uint16_t length)
{
    ble_gatts_hvx_params_t hvx_params;
	
	
    //return sd_ble_gatts_value_set(p_nus->status_handles.value_handle,
		//																		0,
		//																	  &length,
			//																 (uint8_t*)string);

    if (p_nus == NULL)
    {
        return NRF_ERROR_NULL;
    }
    
		
    if ((p_nus->conn_handle == BLE_CONN_HANDLE_INVALID) || (!p_nus->is_notification_enabled))
    {
				return NRF_ERROR_INVALID_PARAM;
		}
    
    if (length > BLE_NUS_MAX_DATA_LEN)
    {
        return NRF_ERROR_INVALID_PARAM;
    }
    
    memset(&hvx_params, 0, sizeof(hvx_params));

    hvx_params.handle = p_nus->status_handles.value_handle;
    hvx_params.p_data = string;
    hvx_params.p_len  = &length;
    hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
    
    return sd_ble_gatts_hvx(p_nus->conn_handle, &hvx_params);
}

uint32_t ble_nus_nonce_get(ble_nus_t * p_nus, uint8_t *data, uint16_t *lenght)
{
	uint32_t err_code = 0;
	if (p_nus == NULL)
    {
        return NRF_ERROR_NULL;
    }
    if(p_nus->conn_handle != BLE_CONN_HANDLE_INVALID)
    {
    	err_code = sd_ble_gatts_value_get(p_nus->nonce_handles.value_handle, 0, lenght, data);
    	if(err_code != NRF_SUCCESS)
    	{
    		return err_code;
    	}
    }
    if (p_nus->conn_handle != BLE_CONN_HANDLE_INVALID)
    {
		ble_gatts_hvx_params_t hvx_params;

		memset(&hvx_params, 0, sizeof(hvx_params));
		hvx_params.handle = p_nus->nonce_handles.value_handle;
		hvx_params.p_data = data;
		hvx_params.p_len  = lenght;
		hvx_params.type   = BLE_GATT_HVX_NOTIFICATION|BLE_GATT_HVX_INDICATION;
		sd_ble_gatts_hvx(p_nus->conn_handle, &hvx_params);
     }
     else
     {
		err_code = NRF_ERROR_INVALID_STATE;
     }
     return err_code;
}

uint32_t ble_nus_send_nonce(ble_nus_t * p_nus, uint8_t * string, uint16_t length)
{
    ble_gatts_hvx_params_t hvx_params;

    if (p_nus == NULL)
    {
        return NRF_ERROR_NULL;
    }
    
    if ((p_nus->conn_handle != BLE_CONN_HANDLE_INVALID) || (!p_nus->is_notification_enabled))
    {
            // Update database
						return sd_ble_gatts_value_set(p_nus->nonce_handles.value_handle,
                                  0,
                                  &length,
                                  (uint8_t*)string);
    }
    
    if (length > BLE_NUS_MAX_DATA_LEN)
    {
        return NRF_ERROR_INVALID_PARAM;
    }
 

    memset(&hvx_params, 0, sizeof(hvx_params));

    hvx_params.handle = p_nus->nonce_handles.value_handle;
    hvx_params.p_data = string;
    hvx_params.p_len  = &length;
    hvx_params.type   = BLE_GATT_HVX_NOTIFICATION|BLE_GATT_HVX_INDICATION;
    
    return sd_ble_gatts_hvx(p_nus->conn_handle, &hvx_params);
		
    
}

