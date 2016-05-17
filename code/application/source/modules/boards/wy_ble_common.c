#include <wy_timers.h>
#include <wy_ble_config.h>
#include <softdevice_handler.h>
#include <app_error.h>
#include <ble_dis.h>
#include <pstorage_platform.h>
#include <ble_conn_params.h>
#include <wy_control_common.h>
#include <wy_ble_common.h>
#include "wy_uart.h"
#include <Ble_gap.h>
#include "wy_tools.h"

static struct ble_manage g_ble_manage;

/****************************/
static void __ble_common_evt_dispatch(ble_evt_t * p_ble_evt);
static H_S32 __ble_common_params_get(ble_gap_sec_params_t *param);
static void __ble_common_sys_evt_dispatch(H_U32 sys_evt);
static void __ble_commeon_on_evt(ble_evt_t * p_ble_evt);
static void __ble_on_conn_params_evt(ble_conn_params_evt_t * p_evt);
static void __ble_conn_params_error_handler(H_U32 nrf_error);
static void __softdevice_callback(void);
static void __ble_connect_config(void);


static struct ble_manage* __ble_manage_get(void)
{
	return (struct ble_manage*)&g_ble_manage;
}

static void __ble_enable(void)
{
	
	H_U32 err_code;
	struct ble_manage* manage = __ble_manage_get();
	// Initialize SoftDevice.
	SOFTDEVICE_HANDLER_INIT(NRF_CLOCK_LFCLKSRC_RC_250_PPM_250MS_CALIBRATION, false);

	// Enable BLE stack 
	ble_enable_params_t ble_enable_params;
	memset(&ble_enable_params, 0, sizeof(ble_enable_params));
	ble_enable_params.gatts_enable_params.service_changed = BLE_CHAR_CHANGED;
	err_code = sd_ble_enable(&ble_enable_params);
	APP_ERROR_CHECK(err_code);
	err_code = sd_ble_gap_tx_power_set(-4);
	APP_ERROR_CHECK(err_code);
}

/*bluetooth device name*/
static void __set_ble_name(void)
{
	H_U32   err_code = 0;
	ble_gap_conn_params_t   gap_conn_params;
	ble_gap_conn_sec_mode_t sec_mode;

	wy_memset(&gap_conn_params, 0, sizeof(ble_gap_conn_params_t));
	wy_memset(&sec_mode, 0, sizeof(ble_gap_conn_sec_mode_t));
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

    char *NAME = BLE_DEVICE_NAME;
	ble_gap_addr_t ADDR;
	sd_ble_gap_address_get(&ADDR);
	
	H_U32 i;
	H_U8 temp[6];
	H_U8 Mac_addr[13] = { 0 }; 	
	for(i = 0;i<BLE_GAP_ADDR_LEN;i++)
	{
		//WYPrintf(MODULE_BLE_COMMON,LEVEL_DEBUG,"MAC ADDR:%c",ADDR->addr[i]);
		temp[i] = ADDR.addr[5-i];
		//WYPrintf(MODULE_BLE_COMMON,LEVEL_DEBUG,"temp:%c",temp[i]);
		//wy_tools_op()->_delay_ms(10);
	}
	//convert the mac addr to hex string
	wy_tools_op()->_to_str(temp,BLE_GAP_ADDR_LEN,Mac_addr,12);//maybe longer than we expect,then may cause crash
	//WYPrintf(MODULE_BLE_COMMON,LEVEL_DEBUG,"mac_addr:%s,lenght:%d",Mac_addr,strlen(Mac_addr));
	H_U8 BLE_NAME[19];//complete name length
	for(i = 0; i < strlen(NAME); i++)
	{
		BLE_NAME[i] = *(NAME+i);
	}
	for(i = 0; i < 12; i++)
	{
		BLE_NAME[i+7] = Mac_addr[i];
	}
	//err_code = sd_ble_gap_device_name_set(&sec_mode,(const H_U8 *) BLE_DEVICE_NAME,
	//                              strlen(BLE_DEVICE_NAME));
	err_code = sd_ble_gap_device_name_set(&sec_mode,BLE_NAME,strlen(BLE_NAME));
	APP_ERROR_CHECK(err_code);

	gap_conn_params.min_conn_interval = BLE_MIN_CONN_INTERVAL;
	gap_conn_params.max_conn_interval = BLE_MAX_CONN_INTERVAL;
	gap_conn_params.slave_latency     = BLE_SLAVE_LATENCY;
	gap_conn_params.conn_sup_timeout  = BLE_CONN_SUP_TIMEOUT;
	err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
	APP_ERROR_CHECK(err_code);
}

/*bluetooth params:*/
static void __set_ble_params(void)
{

	H_U32         err_code;
	ble_dis_init_t dis_init;
	// Initialize Device Information Service
	memset(&dis_init, 0, sizeof(dis_init));

	ble_srv_ascii_to_utf8(&dis_init.manufact_name_str, BLE_MANUFACTURER_NAME);
	ble_srv_ascii_to_utf8(&dis_init.model_num_str, BLE_MOBLE_NUMBER);
	ble_srv_ascii_to_utf8(&dis_init.hw_rev_str, BLE_HARDWARE_VERSION);
	ble_srv_ascii_to_utf8(&dis_init.fw_rev_str, BLE_FIRMWARE_VERSION);
	ble_srv_ascii_to_utf8(&dis_init.sw_rev_str, BLE_SOFTWARE_VERSION);

	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&dis_init.dis_attr_md.read_perm);
	BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&dis_init.dis_attr_md.write_perm);
	err_code = ble_dis_init(&dis_init);
	APP_ERROR_CHECK(err_code);

}

static void __ble_commmon_open(void)
{
	//step1:bluetooth open 
	__ble_enable();

	//step2:set bluetooth name
	__set_ble_name();

	//step3:set bluetooth params
	__set_ble_params();

	g_ble_manage.handler = BLE_CONN_HANDLE_INVALID;
}

static void __ble_connect_config(void)
{
	H_U32               err_code;
    ble_conn_params_init_t cp_init;
    
    memset(&cp_init, 0, sizeof(cp_init));
    cp_init.p_conn_params                  = NULL;
    cp_init.first_conn_params_update_delay = BLE_FIRST_CONN_PARAMS_UPDATE_DELAY;
    cp_init.next_conn_params_update_delay  = BLE_NEXT_CONN_PARAMS_UPDATE_DELAY;
    cp_init.max_conn_params_update_count   = BLE_MAX_CONN_PARAMS_UPDATE_COUNT;
    cp_init.start_on_notify_cccd_handle    = BLE_GATT_HANDLE_INVALID;
    cp_init.disconnect_on_fail             = false;
    cp_init.evt_handler                    = __ble_on_conn_params_evt;
    cp_init.error_handler                  = __ble_conn_params_error_handler;
    
    err_code = ble_conn_params_init(&cp_init);
    APP_ERROR_CHECK(err_code);
}

static void __softdevice_callback(void)
{
	H_S32 err_code = 0;
	// Subscribe for BLE events.
	err_code = softdevice_ble_evt_handler_set(__ble_common_evt_dispatch);
	APP_ERROR_CHECK(err_code);

	// Register with the SoftDevice handler module for BLE events.
	err_code = softdevice_sys_evt_handler_set(__ble_common_sys_evt_dispatch); 
	APP_ERROR_CHECK(err_code);
}

static void __ble_common_evt_dispatch(ble_evt_t * p_ble_evt)
{
    ble_conn_params_on_ble_evt(p_ble_evt);
   // ble_nus_on_ble_evt(&m_nus, p_ble_evt);
    wy_near_control_op()->_near_event(p_ble_evt);
    __ble_commeon_on_evt(p_ble_evt);
}

static H_S32 __ble_common_params_get(ble_gap_sec_params_t *param)
{
	CHECK_PARAMETER(param,H_NULL,H_FAULTER);
	
    param->timeout      = BLE_SEC_PARAM_TIMEOUT;
    param->bond         = BLE_SEC_PARAM_BOND;
    param->mitm         = BLE_SEC_PARAM_MITM;
    param->io_caps      = BLE_SEC_PARAM_IO_CAPABILITIES;
    param->oob          = BLE_SEC_PARAM_OOB;  
    param->min_key_size = BLE_SEC_PARAM_MIN_KEY_SIZE;
    param->max_key_size = BLE_SEC_PARAM_MAX_KEY_SIZE;
	return H_SUCCESS;
}

static void __ble_common_sys_evt_dispatch(H_U32 sys_evt) 
{ 
       pstorage_sys_event_handler(sys_evt); 
} 

static void __ble_commeon_on_evt(ble_evt_t * p_ble_evt)
{
    H_U32                         err_code;
    static ble_gap_evt_auth_status_t m_auth_status;
    ble_gap_enc_info_t *             p_enc_info;
    struct ble_manage* manage = __ble_manage_get();
    ble_gap_sec_params_t gap_param;

    memset(&gap_param, 0, sizeof(ble_gap_sec_params_t));
    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            manage->handler = p_ble_evt->evt.gap_evt.conn_handle;
            wy_near_control_op()->_send_random();
		#ifdef MODULE_WIFI_ENABLE
            _MessageTransfer(_MSG_WIFI_STATUS_BLE_CONNECT,0,0);
		#endif
            break;
        case BLE_GAP_EVT_DISCONNECTED:
            manage->handler = BLE_CONN_HANDLE_INVALID;
            wy_bluetooth_op()->_adv._start();
            wy_near_control_op()->_reset_random();
		#ifdef MODULE_WIFI_ENABLE
            _MessageTransfer(_MSG_WIFI_STATUS_BLE_DISCONNECT,0,0);
		#endif
			break;
        case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
        	__ble_common_params_get(&gap_param);
            err_code = sd_ble_gap_sec_params_reply(manage->handler, 
                                                   BLE_GAP_SEC_STATUS_SUCCESS, 
                                                   &gap_param);
            //APP_ERROR_CHECK(err_code);
            break;
        case BLE_GATTS_EVT_SYS_ATTR_MISSING:
            err_code = sd_ble_gatts_sys_attr_set(manage->handler, NULL, 0);
            //APP_ERROR_CHECK(err_code);
            break;
        case BLE_GAP_EVT_AUTH_STATUS:
            m_auth_status = p_ble_evt->evt.gap_evt.params.auth_status;
            break;
        case BLE_GAP_EVT_SEC_INFO_REQUEST:
            p_enc_info = &m_auth_status.periph_keys.enc_info;
            if (p_enc_info->div == p_ble_evt->evt.gap_evt.params.sec_info_request.div)
            {
                err_code = sd_ble_gap_sec_info_reply(manage->handler, p_enc_info, NULL);
              //  APP_ERROR_CHECK(err_code);
            }
            else
            {
                // No keys found for this device
                err_code = sd_ble_gap_sec_info_reply(manage->handler, NULL, NULL);
                //APP_ERROR_CHECK(err_code);
            }
            break;
        case BLE_GAP_EVT_TIMEOUT:
			wy_bluetooth_op()->_adv._start();
            break;
        default:
            // No implementation needed.
            break;
    }
}


static H_U32 __ble_common_disconnect(void)
{
	H_U32 err_code = 0;
	struct ble_manage* manage = __ble_manage_get();
	if(manage->handler != BLE_CONN_HANDLE_INVALID)
	{
		WYPrintf(MODULE_BLE_COMMON,LEVEL_DEBUG,"disconnecting....");
		err_code = sd_ble_gap_disconnect(manage->handler, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
	}
	return err_code; 
}


static void __ble_on_conn_params_evt(ble_conn_params_evt_t * p_evt)
{
    H_U32 err_code;
    struct ble_manage* manage = __ble_manage_get();
    if(p_evt->evt_type == BLE_CONN_PARAMS_EVT_FAILED)
    {
        err_code = sd_ble_gap_disconnect(manage->handler, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
        APP_ERROR_CHECK(err_code);
    }
}

static void __ble_conn_params_error_handler(H_U32 nrf_error)
{
    APP_ERROR_HANDLER(nrf_error);
}

static H_BOOL __ble_connect(void)
{
	struct ble_manage* manage = __ble_manage_get();
	if(manage->handler != BLE_CONN_HANDLE_INVALID)
	{
		//WYPrintf(MODULE_BLE_COMMON,LEVEL_DEBUG,"__ble_connect   connect");
		return H_TRUE;
	}
	else
	{
		//WYPrintf(MODULE_BLE_COMMON,LEVEL_DEBUG,"__ble_connect   disconnect");
		return H_FALSE;
	}
}


ble_device_t* wy_bluetooth_op(void)
{
	static ble_device_t ___ble_op = {
					._open = __ble_commmon_open,
					._soft_callback = __softdevice_callback,
					._connect_config = __ble_connect_config,
					._disconnect = __ble_common_disconnect,
					._isconnect = __ble_connect,
					._adv = {
						._open = wy_advertising_open,
						._start = wy_advertising_start,
					},
					._character = {
						._start = wy_character_start,
					},
	};
	return &___ble_op;
}


