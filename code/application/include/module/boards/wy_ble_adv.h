
#ifndef __BLE_ADV_H__
#define __BLE_ADV_H__

typedef void (*adv_open)(void);
typedef void (*adv_start)(void);

typedef struct adv_service{
	adv_open _open;
	adv_start _start;
}adv_service_t;

typedef __packed struct 
{ 
	H_U16 unknown1;
	H_U8  uuid[16];
	H_U16 major;
	H_U16 minor;
	H_U8  unknown2; 
}clbeacon_info_t;




void wy_advertising_open(void);
void wy_advertising_start(void);

#endif
