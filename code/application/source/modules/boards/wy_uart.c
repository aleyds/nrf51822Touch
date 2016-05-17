
#include<stdarg.h>
#include "nrf_soc.h"
#include "wy_tools.h"
#include<wy_uart.h>

#if defined MODULE_UART_ENABLE
#define UART_PUT_CHAR_TIMEOUT		(5000)
static app_uart_event_handler_t    g_uart_event_handler = H_NULL;    
static H_U16 g_uart_RX_data = 0xffff; 
static H_U32 g_uart_timeout = 0;



#define check_zero(data)\
do{\
	if(data == 0)\
	{\
		__uart_put((H_U8)('0'));\
		break;\
	}\
}while(0)

static void __uart_config(H_U32 baud, H_U32 parity,H_BOOL hwfc)
{
/*
	H_U8 rts_pin_number = UART_RTS_PORT;
	H_U8 txd_pin_number = UART_TX_PORT;
	H_U8 cts_pin_number = UART_CTS_PORT;
	H_U8 rxd_pin_number = UART_RX_PORT;*/
	/** @snippet [Configure UART RX and TX pin] */
	nrf_gpio_cfg_output(UART_TX_PORT);
	
	nrf_gpio_cfg_input(UART_RX_PORT, NRF_GPIO_PIN_NOPULL);

	NRF_UART0->PSELTXD = UART_TX_PORT;
	NRF_UART0->PSELRXD = UART_RX_PORT;
	/** @snippet [Configure UART RX and TX pin] */
	if (hwfc)
	{
		nrf_gpio_cfg_output(UART_RTS_PORT);
		nrf_gpio_cfg_input(UART_CTS_PORT, NRF_GPIO_PIN_NOPULL);
		NRF_UART0->PSELCTS = UART_CTS_PORT;
		NRF_UART0->PSELRTS = UART_RTS_PORT;
		NRF_UART0->CONFIG  = (UART_CONFIG_HWFC_Enabled << UART_CONFIG_HWFC_Pos);
	}

	NRF_UART0->BAUDRATE 	 = (baud << UART_BAUDRATE_BAUDRATE_Pos);
	NRF_UART0->BAUDRATE 	 = (UART_BAUDRATE_BAUDRATE_Baud38400 << UART_BAUDRATE_BAUDRATE_Pos);
	NRF_UART0->ENABLE		 = (UART_ENABLE_ENABLE_Enabled << UART_ENABLE_ENABLE_Pos);
	NRF_UART0->TASKS_STARTTX = 1;
	NRF_UART0->EVENTS_RXDRDY = 0;


}


void _UartOpen(void)
{
	__uart_config(UART_BAUDRATE_BAUDRATE_Baud38400, 0, UART_HWFC);
}


static void __uart_put(H_U8 ch)
{
	NRF_UART0->TXD = (H_U16)ch;
	
	while (NRF_UART0->EVENTS_TXDRDY != 1)
	{
		// Wait for TXD data to be sent.
		if(g_uart_timeout > UART_PUT_CHAR_TIMEOUT)
		{
			break;
		}
		g_uart_timeout++;
	}
	g_uart_timeout = 0;
	NRF_UART0->EVENTS_TXDRDY = 0;
}




static void __uart_put_str(const H_U8 *str)
{
	while(*str != '\0')
	{
		__uart_put(*str++);
	}
}

static void __uart_put_dec(H_U32 dec)
{
	H_U8 buffer[12] = { 0 };
	sprintf(buffer, "%d", dec);
	__uart_put_str(buffer);
}


static void __uart_put_bin(H_U32 bin)
{
	if(bin == 0)
	{
		__uart_put_str("0b");
		return;
	}
	__uart_put_bin(bin/2);
	__uart_put((char )(bin%2 + '0'));
}

static void __uart_put_hex(H_U32 hex)
{
	H_U8 buffer[12] = { 0 };
	sprintf(buffer, "0x%x", hex);
	__uart_put_str(buffer);
}

void _UartPrintf(H_U8 *fmt, ...)
{
	H_U32 vargint = 0;
	H_U8 *vargpch = H_NULL;
	H_U8 vargch = 0;
	H_U8 *pfmt = H_NULL;

	va_list vp;
	pfmt = fmt;
	va_start(vp, fmt);
	while(*pfmt)
	{
		if(*pfmt == '%')
		{
			switch(*(++pfmt))
			{
				case 'c':
					vargch = va_arg(vp,H_U32);
					__uart_put((H_U8)vargch);
					break;
				case 'd':
				case 'i':
					vargint = va_arg(vp, H_U32);
					check_zero(vargint);
					__uart_put_dec(vargint);
					break;
				case 's':
					vargpch = va_arg(vp, H_U8 *);
					__uart_put_str(vargpch);
					break;
				case 'b':
				case 'B':
					vargint = va_arg(vp, H_U32);
					check_zero(vargint);
					__uart_put_bin(vargint);
					break;
				case 'x':
				case 'X':
					vargint = va_arg(vp, H_U32);
					//check_zero(vargint);
					__uart_put_hex(vargint);
					break;
				case '%':
					__uart_put('%');
					break;
			}
			pfmt++;
		}
		else
		{
			__uart_put(*pfmt++);
		}
	}
	va_end(vp);
}


#else
void _UartClose(void)
{
	NRF_UART0->TASKS_STOPTX = 1;
	NRF_UART0->TASKS_STOPRX = 1;
	NRF_UART0->ENABLE = 0;
}
#endif

