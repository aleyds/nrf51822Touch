

#include <wy_ble_character.h>

void wy_character_start(ble_nus_t *p_nus, ble_nus_data_handler_t handler,H_U8 *nonce_data)
{
	 H_U32         err_code;
    ble_nus_init_t   nus_init;
	//  ble_dis_init_t dis_init;
    
    memset(&nus_init, 0, sizeof(nus_init));

    nus_init.data_handler = handler;
    
    err_code = ble_nus_init(p_nus, &nus_init,nonce_data);
    APP_ERROR_CHECK(err_code);
}

