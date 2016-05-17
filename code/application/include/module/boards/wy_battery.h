
#ifndef __BATTERY_H__
#define __BATTERY_H__
#include <base_type.h>
//#include "ble_bas.h"
#define MODULE_DEBUG_BATTERY	"BATTERY"

#define BATTERY_LEVEL_MIN		(0)
#define BATTERY_LEVEL_MAX		(100)

typedef struct  battery_device{
		H_U32 type;
		function_void _open;
		uint32_function _get_level;
		uint32_function _get_level_direct;
}battery_device_t;

struct battery_manage{
//	ble_bas_t handler;
	H_U32 level;
};
battery_device_t *wy_battery_op(void);

#endif

