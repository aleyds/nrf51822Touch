/**********************************************************************
***********************************************************************/
#include "wy_light.h"

#ifdef MODULE_LED_ENABLE

void _LightOpen(void)
{

#if (TOUCH_LED_TYPE == TOUCH_LED_GUIDE)
	nrf_gpio_cfg_output(LIGHT_LED_BIG);
	nrf_gpio_pin_clear(LIGHT_LED_BIG);
#elif (TOUCH_LED_TYPE == TOUCH_LED_INDEPENDENT)
	nrf_gpio_cfg_output(LIGHT_LED1);
	nrf_gpio_pin_clear(LIGHT_LED1);
#endif
}

void _LightOnOff(const enum _Light_kind type,H_BOOL on)
{
	H_U32 pin_number = 0;
	switch(type)
	{
		case _LIGHT_ADV:
			pin_number = ADVERTISING_LED_PIN_NO;
			break;
		case _LIGHT_RED:
			pin_number = LIGHT_LED_RED;
			break;
		case _LIGHT_GREEN:
			pin_number = LIGHT_LED_GREEN;
			break;
		case _LIGHT_BIG:
			pin_number = LIGHT_LED_BIG;
			break;
		case _LIGHT_LED1:
			pin_number = LIGHT_LED1;
			break;
		default:
			break;
	}
	
	if(on)
	{
		nrf_gpio_pin_set(pin_number);
	}
	else
	{
		nrf_gpio_pin_clear(pin_number);
	}
}


#endif

