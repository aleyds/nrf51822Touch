
#ifndef __BLE_CONFIG_H__
#define __BLE_CONFIG_H__

#define BLE_DEVICE_NAME					"fLock-1"		 /**< Name of device. Will be included in the advertising data. */
#define BLE_MANUFACTURER_NAME 			"HuaShang HT"	 /**< Manufacturer. Will be passed to Device Information Service. */
#define BLE_MOBLE_NUMBER				"fLock-1.0"		 /**< Manufacturer. Will be passed to Device Information Service. */
#define BLE_HARDWARE_VERSION			"0.1"			 /**< Manufacturer. Will be passed to Device Information Service. */
#define BLE_SOFTWARE_VERSION			"0.2"			 /**< Manufacturer. Will be passed to Device Information Service. */
#define BLE_FIRMWARE_VERSION			"5182:v1.5.70.0" /**< Manufacturer. Will be passed to Device Information Service. */


#define BLE_MIN_CONN_INTERVAL           16               /**< Minimum acceptable connection interval (20 ms), Connection interval uses 1.25 ms units. */
#define BLE_MAX_CONN_INTERVAL           80               /**< Maximum acceptable connection interval (75 ms), Connection interval uses 1.25 ms units. */
#define BLE_SLAVE_LATENCY               0                /**< slave latency. */
#define BLE_CONN_SUP_TIMEOUT            400              /**< Connection supervisory timeout (4 seconds), Supervision Timeout uses 10 ms units. */

#define BLE_CHAR_CHANGED				0                /**< Include or not the service_changed characteristic. if not enabled, the server's database cannot be changed for the lifetime of the device*/

#define BLE_ADV_INTERVAL                800//1600             /**< The advertising interval (in units of 0.625 ms. This value corresponds to 40 ms). */
#define BLE_ADV_TIMEOUT_IN_SECONDS      1800             /**< The advertising timeout (in units of seconds). */

#define BLE_FIRST_CONN_PARAMS_UPDATE_DELAY  APP_TIMER_TICKS(5000, APP_TIMER_PRESCALER)  /**< Time from initiating event (connect or start of notification) to first time sd_ble_gap_conn_param_update is called (5 seconds). */
#define BLE_NEXT_CONN_PARAMS_UPDATE_DELAY   APP_TIMER_TICKS(30000, APP_TIMER_PRESCALER) /**< Time between each call to sd_ble_gap_conn_param_update after the first call (30 seconds). */
#define BLE_MAX_CONN_PARAMS_UPDATE_COUNT    3                                           /**< Number of attempts before giving up the connection parameter negotiation. */


#endif

