
#ifndef __UART_H__
#define __UART_H__
#include <base_type.h>
#include <boards.h>
#include <nrf.h>
#include <nrf_gpio.h>
#include <app_util_platform.h>

/**@brief Enumeration which defines events used by the UART module upon data reception or error.
 *
 * @details The event type is used to indicate the type of additional information in the event
 * @ref app_uart_evt_t.
 */
typedef enum
{
    APP_UART_DATA_READY,          /**< An event indicating that UART data has been received. The data is available in the FIFO and can be fetched using @ref app_uart_get. */
    APP_UART_FIFO_ERROR,          /**< An error in the FIFO module used by the app_uart module has occured. The FIFO error code is stored in app_uart_evt_t.data.error_code field. */
    APP_UART_COMMUNICATION_ERROR, /**< An communication error has occured during reception. The error is stored in app_uart_evt_t.data.error_communication field. */
    APP_UART_TX_EMPTY,            /**< An event indicating that UART has completed transmission of all available data in the TX FIFO. */
    APP_UART_DATA,                /**< An event indicating that UART data has been received, and data is present in data field. This event is only used when no FIFO is configured. */
} app_uart_evt_type_t;

/**@brief Struct containing events from the UART module.
 *
 * @details The app_uart_evt_t is used to notify the application of asynchronous events when data
 * are received on the UART peripheral or in case an error occured during data reception.
 */
typedef struct
{
    app_uart_evt_type_t evt_type; /**< Type of event. */
    union
    {
        H_U32 error_communication; /**< Field used if evt_type is: APP_UART_COMMUNICATION_ERROR. This field contains the value in the ERRORSRC register for the UART peripheral. The UART_ERRORSRC_x defines from @ref nrf51_bitfields.h can be used to parse the error code. See also the nRF51 Series Reference Manual for specification. */
        H_U32 error_code;          /**< Field used if evt_type is: NRF_ERROR_x. Additional status/error code if the error event type is APP_UART_FIFO_ERROR. This error code refer to errors defined in nrf_error.h. */
        H_U8  value;               /**< Field used if evt_type is: NRF_ERROR_x. Additional status/error code if the error event type is APP_UART_FIFO_ERROR. This error code refer to errors defined in nrf_error.h. */
    } data;
} app_uart_evt_t;

/**@brief Function for handling app_uart event callback.
 *
 * @details Upon an event in the app_uart module this callback function will be called to notify
 *          the applicatioon about the event.
 *
 * @param[in]   p_app_uart_event Pointer to UART event.
 */


typedef void (* app_uart_event_handler_t) (app_uart_evt_t * p_app_uart_event);


#if defined MODULE_UART_ENABLE

#define UART_DEBUG 1
#ifdef UART_DEBUG
#else
#endif

#define UART_HWFC		(H_FALSE)

void _UartOpen(void);
void _UartPrintf(H_U8 *fmt, ...);


#else
void _UartClose(void);

#endif


#endif
