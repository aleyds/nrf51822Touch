
#ifndef __LIB_SPI_H__
#define __LIB_SPI_H__
#include <base_type.h>

#define LIB_SPI_MODULE			"SPI_LIB"
#ifdef SPI_MASTER_0_ENABLE
#define HARDWARE_INSTANCE		(SPI_MASTER_0)
#else
#define HARDWARE_INSTANCE		(SPI_MASTER_1)
#endif
#define DEBUG_PIN_SWITCH

#define LIB_SPIM0_SCK_PIN							(SPIM0_SCK_PORT)			/*SPI Clock*/
#define LIB_SPIM0_MOSI_PIN							(SPIM0_MOSI_PORT)			/*Master out Slave in*/
#define LIB_SPIM0_MISO_PIN							(SPIM0_MISO_PORT)		/*Master in Slave out NOT USE*/

#define LIB_VOICE_CHIP_CS							(SPI_VOICE_CS_PORT)
#define LIB_WIFI_CHIP_CS							(SPI_CC3100_CS_PORT)
#define LIB_WIFI_INT_PIN							(SPI_INT_CC3100_PORT)
#define LIB_WIFI_VCC_SWITCH							(SPI_CC3100_SWITCH_PORT)
#define LIB_WIFI_PWMB_SWITCH						(SPI_CC3100_PWMB_PORT)

#define LIB_SPIM0_SS_PIN							(SPIM0_SS_PORT)		/*Slave Select*/
#define LIB_SPI_ERROR_PIN							(GPIO_PIN8)
#ifdef DEBUG_PIN_SWITCH
#define LIB_SPI_DEBUG_EVENT_PIN0					(GPIO_PIN10)
#endif

#define LIB_SPI_MSG_LEN				(100)				/*transmit and receive lenght*/


H_U8 lib_spi_open( H_U8 type);
H_U32 lib_spi_send_recv(H_U8 *p_tx_data, H_U8* p_rx_data, H_U32 len,H_U8 type);
H_U32 lib_spi_destroy(void);
H_U8 lib_spi_occupy(void);

#ifdef MODULE_WIFI_ENABLE
void CC3100_enable(void);
void CC3100_disable(void);

#endif

#endif

