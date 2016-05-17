
#include <wy_ble_config.h>
#include <base_type.h>
#include <ble_advdata.h>
#include <ble_nus.h>
#include <app_error.h>
#include <Ble_gap.h>
#include "wy_uart.h"
#include "wy_ble_adv.h"

void wy_advertising_open(void)
{
    H_U32 err_code;
    ble_advdata_t advdata;
    ble_advdata_t scanrsp;
    H_U8 flags = BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE;
    //ble_uuid_t adv_uuids[] = {{BLE_UUID_INF_ALERM_SERVICE, BLE_UUID_TYPE_BLE}};
	H_U8 uuid[16] ={0xe2,0xc5,0x6d,0xb5,0xdf,0xfb,0x48,0xd2,0xb0,0x60,0xD2,0x8F,0xE8,0x92,0x44,0x73};

	clbeacon_info_t beacon_info;
	memset(&beacon_info, 0, sizeof(beacon_info));
	//WYPrintf("ble_adv",LEVEL_DEBUG,"size of beacon_info0:%d",sizeof(beacon_info));
	beacon_info.unknown1 = 0x1502; //length ,DataType
	memcpy(&beacon_info.uuid,&uuid,sizeof(uuid));
	//major will be the first two bytes of mac address
	ble_gap_addr_t ADDR;
	sd_ble_gap_address_get(&ADDR);
	beacon_info.major = ((ADDR.addr[4]<<8)&0xFF00)+ADDR.addr[5];
	beacon_info.minor = 0;
	beacon_info.unknown2 = 0xB4; //TX power

	ble_advdata_manuf_data_t manuf_specific_data;
	manuf_specific_data.company_identifier = 0x004C; 
	manuf_specific_data.data.p_data = (H_U8*)&beacon_info;
	manuf_specific_data.data.size = sizeof(beacon_info);
	//WYPrintf("ble_adv",LEVEL_DEBUG,"size of beacon_info1:%d",sizeof(beacon_info));

    memset(&advdata, 0, sizeof(advdata));
    advdata.name_type               = BLE_ADVDATA_NO_NAME;
    advdata.include_appearance      = false;
    advdata.flags.size              = sizeof(flags);
    advdata.flags.p_data            = &flags;
	advdata.p_manuf_specific_data = &manuf_specific_data;
    memset(&scanrsp, 0, sizeof(scanrsp));
    //scanrsp.uuids_complete.uuid_cnt = sizeof(adv_uuids) / sizeof(adv_uuids[0]);
    //scanrsp.uuids_complete.p_uuids  = adv_uuids;
	scanrsp.name_type               = BLE_ADVDATA_FULL_NAME;
	scanrsp.include_appearance      = false;
	//name set will be at wy_ble_common.c ,set_ble_name() 
	
    err_code = ble_advdata_set(&advdata, &scanrsp);
    APP_ERROR_CHECK(err_code);

	//	H_U32 i;
	//for(i=0;i<BLE_GAP_ADDR_LEN;i++)
	//WYPrintf("BLE_adv",LEVEL_DEBUG,"MAC ADDR:%c",ADDR->addr[i]);
	
}

void wy_advertising_start(void)
{
    H_U32             err_code;
    ble_gap_adv_params_t adv_params;
    
    // Start advertising
    memset(&adv_params, 0, sizeof(adv_params));
    
    adv_params.type        = BLE_GAP_ADV_TYPE_ADV_IND;
    adv_params.p_peer_addr = NULL;
    adv_params.fp          = BLE_GAP_ADV_FP_ANY;
    adv_params.interval    = BLE_ADV_INTERVAL;
    adv_params.timeout     = BLE_ADV_TIMEOUT_IN_SECONDS;

    err_code = sd_ble_gap_adv_start(&adv_params);
    APP_ERROR_CHECK(err_code);

   // nrf_gpio_pin_set(ADVERTISING_LED_PIN_NO);
}


