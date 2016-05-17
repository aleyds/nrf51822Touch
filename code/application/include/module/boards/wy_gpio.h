#ifndef __GPIO_H__
#define __GPIO_H__
#include <base_type.h>

#define GPIO_AMOUNT			(32)

/*param1:由低到高电平转换  param2:由低到高电平转换*/
typedef void (*gpio_event_handler)(H_U32 ,H_U32);


typedef struct gpio_user{
	H_U32 pin_mask;
	H_U32 pin_number;
	H_U32 pins_low_to_high_mask;
	H_U32 pins_high_to_low_mask;
	H_U32 sense_high_pins;
	gpio_event_handler _callback;
}gpio_user_t;

typedef H_U32 (*gpio_onoff)(H_U32 , H_BOOL);

struct gpio_list{
	struct list_head entries;
	H_U32 user_number;
	gpio_user_t user_info;
};
typedef H_U32 (*gpio_register)(struct gpio_list *);

typedef struct gpio_device{
	H_U32 type;
	uint32_function _open;
	gpio_register _register;
	gpio_onoff _onoff;
}gpio_device_t;

struct gpio_manage{
	H_U32 gpio_mask;
};

void _GpioOpen(void);
void _GpioJoin(struct gpio_list *list);
H_U32 _GpioOnOff(H_U32 Pin, H_BOOL en);



#endif

