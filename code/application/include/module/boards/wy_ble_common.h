
#ifndef __BLE_COMMON_H__
#define __BLE_COMMON_H__
#include <base_type.h>
#include <ble_hci.h>
#include <wy_ble_adv.h>
#include <wy_ble_character.h>

#define MODULE_BLE_COMMON			"BLE_COMMON"

#define BLE_COMMON_HANDLE_INVALID BLE_CONN_HANDLE_INVALID

#define BLE_SEC_PARAM_TIMEOUT               30                                          /**< Timeout for Pairing Request or Security Request (in seconds). */
#define BLE_SEC_PARAM_BOND                  1                                           /**< Perform bonding. */
#define BLE_SEC_PARAM_MITM                  0                                           /**< Man In The Middle protection not required. */
#define BLE_SEC_PARAM_IO_CAPABILITIES       BLE_GAP_IO_CAPS_NONE                        /**< No I/O capabilities. */
#define BLE_SEC_PARAM_OOB                   0                                           /**< Out Of Band data not available. */
#define BLE_SEC_PARAM_MIN_KEY_SIZE          7                                           /**< Minimum encryption key size. */
#define BLE_SEC_PARAM_MAX_KEY_SIZE          16                                          /**< Maximum encryption key size. */


typedef void (*open)(void);
typedef void (*softdevice_cb)(void);
typedef void (*connect_config)(void);
typedef H_BOOL (*ble_isconnect)(void);


typedef struct ble_device{
	H_U32 type;
	open _open;
	softdevice_cb _soft_callback;
	connect_config _connect_config;
	uint32_function _disconnect;
	ble_isconnect _isconnect;
	adv_service_t _adv;
	character_service_t _character;
}ble_device_t;

struct ble_manage{
	H_U16 handler;
	
};

ble_device_t* wy_bluetooth_op(void);

#endif
