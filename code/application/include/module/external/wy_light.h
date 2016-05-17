/**********************************************************************
***********************************************************************/
#ifndef __LIGHT_H__
#define __LIGHT_H__
#include <base_type.h>
#include <boards.h>
#include <app_error.h>
#include <nrf_gpio.h>


#define LIG_DEBUG 1
#ifdef LIG_DEBUG
#else
#endif

#define ADVERTISING_LED_PIN_NO          LED_ADV_PORT                         /**< LED to indicate advertising state. */
#define LIGHT_LED_RED					(LIGHT_RED_PORT)
#define LIGHT_LED_GREEN					(LIGHT_GREEN_PORT)
#define LIGHT_LED_BIG					(LIGHT_LED_BIG_PORT)
#define LIGHT_LED1						(LIGHT_LED1_PORT)


enum _Light_kind{
	_LIGHT_ADV = 0x01,
	_LIGHT_RED,
	_LIGHT_GREEN,
	_LIGHT_BIG,
	_LIGHT_LED1,
};


void _LightOpen(void);
void _LightOnOff(const enum _Light_kind type,H_BOOL on);


#endif

