/* Copyright (c) 2012 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */
#ifndef PCA10001_H
#define PCA10001_H

#include "nrf_gpio.h"

/**********Available GPIO PIN#*********/
#define GPIO_PIN0		(0)			/*LED*/	
#define GPIO_PIN1		(1)			/*Finger INT*/
#define GPIO_PIN2		(2)			/*LED1*/
#define GPIO_PIN3		(3)			/*BATTERY*/
#define GPIO_PIN4		(4)			/*PWMA*/
#define GPIO_PIN5		(5)			/*AIN2*/
#define GPIO_PIN6		(6)			/*AIN1*/
#define GPIO_PIN7		(7)			/*STBY*/
#define GPIO_PIN8		(8)			/*I2C SDA*/
#define GPIO_PIN9		(9)			/*I2C SCL*/
#define GPIO_PIN10		(10)		/*Not used*/
#define GPIO_PIN11		(11)		/*Not used*/
#define GPIO_PIN12		(12)		/*OUT3*/
#define GPIO_PIN13		(13)		/*OUT1*/
#define GPIO_PIN14		(14)		/*SPI CLK*/
#define GPIO_PIN15		(15)		/*SPI MOSI*/
#define GPIO_PIN16		(16)		/*Not used*/
#define GPIO_PIN17		(17)		/*Not used*/
#define GPIO_PIN18		(18)		/*BUZZER*/
#define GPIO_PIN19		(19)		/*SPI CS*/
#define GPIO_PIN20		(20)		/*Not used*/
#define GPIO_PIN21		(21)		/*Not used*/
#define GPIO_PIN22		(22)		/*Not used*/
#define GPIO_PIN23		(23)		/*Not used*/
#define GPIO_PIN24		(24)		/*Not used*/
#define GPIO_PIN25		(25)		/*Not used*/
#define GPIO_PIN26		(26)		/*Not used*/
#define GPIO_PIN27		(27)		/*KEYBOARD INT*/
#define GPIO_PIN28		(28)		/*FINGER ENABLE*/
#define GPIO_PIN29		(29)		/*UART RX*/
#define GPIO_PIN30		(30)		/*UART TX*/
#define GPIO_NULL		(31)

/*****************define  Light Ports****/
#define LED_ADV_PORT			(GPIO_NULL)
#define LIGHT_RED_PORT			(GPIO_NULL)
#define LIGHT_GREEN_PORT		(GPIO_NULL)
#define LIGHT_LED_BIG_PORT		(GPIO_PIN0) /*·Ç´¥ÃþI2C LEDÒý½Å*/
#define LIGHT_LED1_PORT			(GPIO_PIN2)  /*´¥ÃþÊ½I2C LEDÒý½Å*/

/******************define  UART Ports******/
#define UART_RX_PORT			(GPIO_PIN30)//GPIO_PIN30
#define UART_TX_PORT			(GPIO_PIN29)//GPIO_PIN29
#define UART_CTS_PORT			(GPIO_NULL)
#define UART_RTS_PORT			(GPIO_NULL)
#define FINGER_ENABLE_PORT		(GPIO_PIN28)
#define FINGER_INTERRUPT		(GPIO_PIN1)

/******************define I2C Ports********/
#define I2C_SCL_PORT			(GPIO_PIN9)
#define I2C_SDL_PORT			(GPIO_PIN8)

/******************define SPI Ports*********/
#define SPIM0_SCK_PORT			(GPIO_PIN14)
#define SPIM0_MOSI_PORT			(GPIO_PIN15)
#define SPIM0_MISO_PORT			(GPIO_NULL)
#define SPI_VOICE_CS_PORT		(GPIO_PIN19)
#define SPI_CC3100_CS_PORT		(GPIO_NULL)
#define SPI_INT_CC3100_PORT		(GPIO_NULL)
#define SPI_CC3100_SWITCH_PORT	(GPIO_NULL)
#define SPI_CC3100_PWMB_PORT	(GPIO_NULL)
#define SPIM0_SS_PORT			(GPIO_NULL)

#define SPIM1_SCK_PORT			(GPIO_NULL)
#define SPIM1_MOSI_PORT			(GPIO_NULL)
#define SPIM1_MISO_PORT			(GPIO_NULL)
#define SPIM1_SS_PORT			(GPIO_NULL)


/*************Cabinet lock keyboard ports******************/
#define KEYBOARD_KEY7           (GPIO_NULL)
#define KEYBOARD_KEY6           (GPIO_NULL)
#define KEYBOARD_KEY5           (GPIO_NULL)
#define KEYBOARD_KEY4           (GPIO_NULL)
#define KEYBOARD_KEY3           (GPIO_NULL)
#define KEYBOARD_KEY2           (GPIO_NULL)
#define KEYBOARD_KEY1           (GPIO_NULL)


/*************Cabinet lock other ports******************/
#define SOLENOID                (GPIO_NULL)
#define TOUCH_TTY6952_IRQ       (GPIO_PIN27)


/******************define DS2465  Ports*******/
#define DS2465_ENABLE_PORT		(GPIO_NULL)//(GPIO_PIN10)
#define DS2465_1WIRE_PORT		(GPIO_NULL)//(GPIO_PIN11)

/******************define Alarm Ports********/
#define ALARM_PORT				(GPIO_PIN18)
#define BIG_ALARM_BUZZER		(GPIO_NULL)

/******************define Battery Ports*******/
#define BATTERY_PORT			(GPIO_PIN3)

/******************define Infrared(IR) Ports****/
#define INFRARED_PORT			(GPIO_NULL)

/******************define Buttons Ports*********/
#define BUTTON_PORT				(GPIO_NULL)

/*******************define Door status pins*******/
#define LOCK_STATE_PORT			(GPIO_NULL)//(GPIO_PIN28)	/*Ëø×´Ì¬*/
#define DOOR_STATE_PORT			(GPIO_NULL)//(GPIO_PIN27)	/*ÃÅ×´Ì¬*/

/*******************define PWM pins***************/
#define STANDBY_PORT			(GPIO_PIN7)
#define MOTOR_AIN1_PORT			(GPIO_PIN5)
#define MOTOR_AIN2_PORT			(GPIO_PIN6)
#define MOTOR_PWMA_PORT			(GPIO_PIN4)
#define MOTOR_BIN1_PORT			(GPIO_NULL)
#define MOTOR_BIN2_PORT			(GPIO_NULL)
#define MOTOR_PWMB_PORT			(GPIO_NULL)
#define MOTOR_SOLENOID_PORT		(GPIO_NULL)

/******************voice chip pins***************/
#define VOICE_CHIP_BUSY_PORT    (GPIO_PIN13)
#define VOICE_CHIP_OUT3_PORT    (GPIO_PIN12)

#endif
