#include <app_error.h>
#include <app_util.h>
#include "wy_tools.h"
#include "wy_uart.h"
#include <battery.h>
#include "wy_battery.h"

#ifdef MODULE_BATTERY_ENABLE

/***********************function predefined********************************/
static H_U32 __battery_level(void);

/***********************global variable**********************************/
static struct battery_manage g_battery_manage;


/***********************function implementation**************************/
static struct battery_manage* __battery_manage_get(void)
{
	return (struct battery_manage *)&g_battery_manage;
}

static void __battery_open(void)
{
    __battery_level();
}

static H_U32 __battery_updata(const H_U16 mvolts)
{
	H_U8 	percentage_batt_lvl = 0;
//	H_U32	err_code = 0;
	struct battery_manage *manage = __battery_manage_get();
	percentage_batt_lvl     = battery_level_in_percent(mvolts);
	manage->level = percentage_batt_lvl;
	WYPrintf(MODULE_DEBUG_BATTERY , LEVEL_DEBUG, "battery level:%d  mvolts:%d",manage->level,mvolts);

//	err_code = ble_bas_battery_level_update(&(manage->handler), percentage_batt_lvl);
//	if ((err_code != NRF_SUCCESS)&&(err_code != NRF_ERROR_INVALID_STATE)
//		 &&(err_code != BLE_ERROR_NO_TX_BUFFERS)&&(err_code != BLE_ERROR_GATTS_SYS_ATTR_MISSING))
//	{
//		return H_FAULTER;
//	}
	return H_SUCCESS;
}

static H_U32 __battery_level(void)
{
	struct battery_manage *manage = __battery_manage_get();
	battery_start(__battery_updata);
	
	return manage->level;
}

static H_U32 __battery_level_direct(void)
{
	struct battery_manage *manage = __battery_manage_get();	
	return manage->level;
}


battery_device_t *wy_battery_op(void)
{
	static battery_device_t ___battery_op = {
			._open = __battery_open,
			._get_level = __battery_level,
			._get_level_direct = __battery_level_direct, 
	};
	return &___battery_op;
}


#endif

