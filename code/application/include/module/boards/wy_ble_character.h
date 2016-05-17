
#ifndef __BLE_CHARACTER_H__
#define __BLE_CHARACTER_H__

#include <base_type.h>
#include <app_error.h>
#include <ble_nus.h>

typedef void (*char_start)(ble_nus_t *,ble_nus_data_handler_t,H_U8 *);

typedef struct character_service{
	char_start _start;
}character_service_t;

void wy_character_start(ble_nus_t *p_nus, ble_nus_data_handler_t handler,H_U8 *nonce_data);

#endif

