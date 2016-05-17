#include <nrf_gpio.h>
#include "spi_master.h"

#include <lib_spi.h>
#include "wy_tools.h"
#include <wy_uart.h>

#undef API_SYNC

#ifdef MODULE_WIFI_ENABLE
void CC3100_InterruptEnable(void);
void CC3100_InterruptDisable(void);
#endif

typedef enum {
	_SPI_NULL,
	_SPI_OPEN,
	_SPI_OPEN_WAIT,
	_SPI_SENDING,
	_SPI_SEND_FINISH,
	_SPI_CLOSE,
}em_spi_stauts;




typedef struct spi_param{
	H_U32 freq;
	H_U8 lsb:1;
	H_U8 cs_trigger:1;/*片选有效方式 0低电平有效  1:高电平有效*/
	H_U8 is_open:1;
	H_U8 cs_pin:5;
	em_spi_stauts spi_stauts;
}spi_param_t;

typedef struct spi_status{
	H_BOOL is_open;
	H_U32 cs_pin;
	H_U8 cs_trigger;
	em_spi_stauts spi_stauts;
}spi_status_t;
static spi_status_t g_spi_status;

#ifdef API_SYNC
static H_BOOL g_spi_mutex = H_FALSE;
#endif

enum {
	_TYPE_VOICE,
	_TYPE_CC3100,
};



#ifdef API_SYNC
static void _spi_lock(void)
{
	if(g_spi_mutex == H_TRUE)
	{
		while(1)
		{
			if(g_spi_mutex == H_FALSE)
			{
				break;
			}
			wy_tools_op()->_delay_ms(5);
		}
	}
	
}

static void _spi_unlock(void)
{
	g_spi_mutex = H_FALSE;
}
#endif

static spi_status_t* __spi_status_get(void)
{
	return (spi_status_t*)&g_spi_status;
}

static void __spi_handler(spi_master_evt_t spi_master_evt)
{
//	H_BOOL result = H_FALSE;
  spi_status_t *status = __spi_status_get();


	switch(spi_master_evt.evt_type)
	{
		case SPI_MASTER_EVT_TRANSFER_STARTED:
			status->spi_stauts = _SPI_SENDING;
			break;
		case SPI_MASTER_EVT_TRANSFER_COMPLETED: /*传输完成*/
			status->spi_stauts = _SPI_SEND_FINISH;
			lib_spi_destroy();
			break;
		case SPI_MASTER_EVT_TYPE_MAX:
			break;
		default:
			break;
	}
}

static H_S32 __spi_replace(H_U32 pin)
{
	spi_status_t *status = __spi_status_get();
	if(((status->spi_stauts == _SPI_OPEN_WAIT)||(status->spi_stauts == _SPI_SEND_FINISH)) 
		&& (status->cs_pin != pin))
	{
		WYPrintf(LIB_SPI_MODULE, LEVEL_DEBUG, "__spi_replace");
		lib_spi_destroy();
	}
	return H_SUCCESS;
}

H_U32 lib_spi_open_ext(const spi_param_t *param)
{
	H_U32 ret_code = 0;
	spi_status_t *status = __spi_status_get();
	spi_master_config_t spi_config = SPI_MASTER_INIT_DEFAULT;
	
	__spi_replace(param->cs_pin);

	
#ifdef API_SYNC
	_spi_lock();
#endif
	
	
	status->is_open = 1;
	status->spi_stauts = _SPI_OPEN;
	status->cs_pin = param->cs_pin;
	status->cs_trigger = param->cs_trigger;
	if(param->freq == 0)
	{
		spi_config.SPI_Freq = SPI_FREQUENCY_FREQUENCY_K125;
	}
	else
	{
		spi_config.SPI_Freq = param->freq;
	}
	spi_config.SPI_Pin_SCK = LIB_SPIM0_SCK_PIN;
	spi_config.SPI_Pin_MISO = LIB_SPIM0_MISO_PIN;
	spi_config.SPI_Pin_MOSI = LIB_SPIM0_MOSI_PIN;
	spi_config.SPI_Pin_SS = param->cs_pin;
	//WYPrintf(LIB_SPI_MODULE, LEVEL_DEBUG, "lib_spi_open_ext  CS_%d",spi_config.SPI_Pin_SS);
	spi_config.SPI_CONFIG_ORDER = ((param->lsb == 1) ? SPI_CONFIG_ORDER_LsbFirst : SPI_CONFIG_ORDER_MsbFirst);
	
	ret_code = spi_master_open(HARDWARE_INSTANCE, &spi_config,param->cs_trigger);
	if(ret_code != NRF_SUCCESS)
	{
#ifdef API_SYNC
		_spi_unlock();
#endif
		return H_FAULTER;
	}

#ifdef API_SYNC
	_spi_unlock();
#endif
	//WYPrintf(LIB_SPI_MODULE, LEVEL_DEBUG, "lib_spi_open_ext");
	spi_master_evt_handler_reg(HARDWARE_INSTANCE, __spi_handler);
	status->spi_stauts = _SPI_OPEN_WAIT;
	return H_SUCCESS;
}

H_U8 lib_spi_open(H_U8 type)
{
	spi_status_t *status = __spi_status_get();
	spi_param_t spi_param;
	
	wy_memset(&spi_param, 0 , sizeof(spi_param_t));
	wy_memset(status, 0 , sizeof(spi_status_t));
	switch(type)
	{
		case _TYPE_VOICE:
			spi_param.freq = SPI_FREQUENCY_FREQUENCY_K125;
			spi_param.cs_trigger = 1;
			spi_param.lsb = 1;
			spi_param.cs_pin = SPI_VOICE_CS_PORT;
			//WYPrintf(LIB_SPI_MODULE, LEVEL_DEBUG, "_TYPE_VOICE");
			break;
		case _TYPE_CC3100:
			spi_param.freq = SPI_FREQUENCY_FREQUENCY_M1;
			spi_param.cs_trigger = 0;
			spi_param.lsb = 0;
			spi_param.cs_pin = SPI_CC3100_CS_PORT;
			//WYPrintf(LIB_SPI_MODULE, LEVEL_DEBUG, "_TYPE_CC3100");
			break;
		default:
			break;
	}
	lib_spi_open_ext(&spi_param);
	return spi_param.cs_trigger;


}

H_U32 lib_spi_destroy(void)
{
	spi_status_t *status = __spi_status_get();
	if(status->is_open == 1)
	{
		status->is_open = 0;
		
		spi_master_close(HARDWARE_INSTANCE,status->cs_trigger);
		status->spi_stauts = _SPI_CLOSE;
		//WYPrintf(LIB_SPI_MODULE, LEVEL_DEBUG, "spi_master_close");
#ifdef API_SYNC
		_spi_unlock();
#endif
		wy_tools_op()->_delay_us(100);
		
	}
	
	return H_SUCCESS;
}

H_U8 lib_spi_occupy(void)
{
	return spi_master_occupy(HARDWARE_INSTANCE);
}

H_U32 lib_spi_send_recv(H_U8 *p_tx_data, H_U8* p_rx_data, H_U32 len,H_U8 type)
{
	spi_status_t *status = __spi_status_get();
	H_U8 cs_trigger = 0;
	//WYPrintf(LIB_SPI_MODULE, LEVEL_DEBUG, "lib_spi_send_recv  1111");
	if(status->is_open == 0)
	{
		cs_trigger = lib_spi_open(type);
	}
	else
	{
		lib_spi_destroy();
		cs_trigger = lib_spi_open(type);
		//cs_trigger = status->cs_trigger;
	}
#ifdef API_SYNC
	_spi_lock();
#endif
	static H_U8 *spi_data_buffer  = H_NULL;
	if(spi_data_buffer != H_NULL)
	{
		wy_free(spi_data_buffer);
		spi_data_buffer = H_NULL;
	}
	spi_data_buffer = wy_malloc(len);

	
	if((H_NULL == p_tx_data) && (H_NULL != spi_data_buffer))
	{
		wy_memset(spi_data_buffer,0,len);
	}
	else
	{
		wy_memcpy(spi_data_buffer, p_tx_data, len);
	}
	spi_master_send_recv(HARDWARE_INSTANCE, spi_data_buffer, len, p_rx_data, len, cs_trigger);
	wy_tools_op()->_delay_us(200);
	
    return H_SUCCESS;
}

#ifdef MODULE_WIFI_ENABLE

void CC3100Hiberbate(H_BOOL hib)
{
	nrf_gpio_cfg_output(LIB_WIFI_PWMB_SWITCH);
	if(hib)
	{	
		nrf_gpio_pin_set(LIB_WIFI_PWMB_SWITCH);
	}
	else
	{
		nrf_gpio_pin_clear(LIB_WIFI_PWMB_SWITCH);
	}
}


void cc3100_vcc_switch(H_BOOL open)
{
	nrf_gpio_cfg_output(LIB_WIFI_VCC_SWITCH);
	if(open)
	{
		nrf_gpio_pin_set(LIB_WIFI_VCC_SWITCH);
	}
	else
	{
		nrf_gpio_pin_clear(LIB_WIFI_VCC_SWITCH);
	}
}

void CC3100_enable(void)
{	
	CC3100Hiberbate(H_TRUE);
	spi_master_chip_cs(HARDWARE_INSTANCE , H_FALSE);
	//WYPrintf(LIB_SPI_MODULE, LEVEL_DEBUG, "CC3100_enable");
}

void CC3100_disable(void)
{
	CC3100Hiberbate(H_FALSE);
	spi_master_chip_cs(HARDWARE_INSTANCE , H_TRUE);
	//WYPrintf(LIB_SPI_MODULE, LEVEL_DEBUG, "CC3100_disable");
}

extern H_U32 wy_cc3100_gpio(H_U8 enable);
void CC3100_InterruptEnable(void)
{
	wy_cc3100_gpio(1);/*使能CC3100中断*/
    
}


void CC3100_InterruptDisable()
{
    wy_cc3100_gpio(0);/**/
}
#endif

