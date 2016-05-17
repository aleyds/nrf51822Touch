#include <config.h>

#ifdef LIB_RTC_ENABLE
#include <stdbool.h>
#include "nrf.h"
#include "nrf_gpio.h"
#include "boards.h"
#include "lib_rtc.h"
#include "wy_uart.h"

#define MODULE_RTC			"LIB_RTC"

/*
static void lfclk_config(void)
{
    NRF_CLOCK->LFCLKSRC            = (CLOCK_LFCLKSRC_SRC_Xtal << CLOCK_LFCLKSRC_SRC_Pos);
    NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_LFCLKSTART    = 1;
    while (NRF_CLOCK->EVENTS_LFCLKSTARTED == 0)
    {
        //Do nothing.
    }
    NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
}
*/

/** @brief Function for configuring the RTC with TICK to 100Hz and COMPARE0 to 10 sec.
 */
static void rtc_config(void)
{
	WYPrintf(MODULE_RTC,LEVEL_DEBUG,"RTC config aaa");
    NRF_RTC0->PRESCALER = COUNTER_PRESCALER;                    // Set prescaler to a TICK of RTC_FREQUENCY.
    NVIC_SetPriority(RTC0_IRQn, RTC0_IRQ_PRI);
    NRF_RTC0->CC[0]     = COMPARE_COUNTERTIME * RTC_FREQUENCY;  // Compare0 after approx COMPARE_COUNTERTIME seconds.
	WYPrintf(MODULE_RTC,LEVEL_DEBUG,"RTC config bbb");
    // Enable TICK event and TICK interrupt:
    //NRF_RTC0->EVTENSET = RTC_EVTENSET_TICK_Msk;
    //NRF_RTC0->INTENSET = RTC_INTENSET_TICK_Msk;
	WYPrintf(MODULE_RTC,LEVEL_DEBUG,"RTC config ccc");

    // Enable COMPARE0 event and COMPARE0 interrupt:
    NRF_RTC0->EVTENSET = RTC_EVTEN_COMPARE0_Msk;
    NRF_RTC0->INTENSET = RTC_INTENSET_COMPARE0_Msk;
    WYPrintf(MODULE_RTC,LEVEL_DEBUG,"RTC config ddd");
    NVIC_EnableIRQ(RTC0_IRQn);                                  // Enable Interrupt for the RTC in the core.
    WYPrintf(MODULE_RTC,LEVEL_DEBUG,"RTC config eee");
}



uint32_t lib_rtc_init(void)
{
	//lfclk_config();
	WYPrintf(MODULE_RTC,LEVEL_DEBUG,"RTC Init start");
    rtc_config();
    WYPrintf(MODULE_RTC,LEVEL_DEBUG,"RTC Init end");
    NRF_RTC0->TASKS_START = 1;
		return 0;
}

/** @brief: Function for handling the RTC0 interrupts.
 * Triggered on TICK and COMPARE0 match.
 */
void RTC0_IRQHandler()
{
	WYPrintf(MODULE_RTC, LEVEL_DEBUG , "RTC0 IRQ handler");
    if ((NRF_RTC0->EVENTS_TICK != 0) &&
        ((NRF_RTC0->INTENSET & RTC_INTENSET_TICK_Msk) != 0))
    {
    	uint32_t tick = NRF_RTC0->EVENTS_TICK;
        NRF_RTC0->EVENTS_TICK = 0;
    }
    
    if ((NRF_RTC0->EVENTS_COMPARE[0] != 0) &&
        ((NRF_RTC0->INTENSET & RTC_INTENSET_COMPARE0_Msk) != 0))
    {
        NRF_RTC0->EVENTS_COMPARE[0] = 0;
        
    }
}
#endif

