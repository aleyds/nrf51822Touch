
#include <nrf_gpio.h>
#include "wy_uart.h"
#include <list.h>
#include "nrf_soc.h"

#include <wy_gpio.h>
static LIST_HEAD(g_gpio_user);
static struct gpio_manage g_gpio_manage;
static H_U32 __gpio_sense_disable(H_U32 pin_number);

static struct gpio_manage* __gpio_manage_get(void)
{
	return (struct gpio_manage *)&g_gpio_manage;
}

void _GpioOpen(void)
{
	wy_memset(&g_gpio_manage, 0 , sizeof(struct gpio_manage));
	// Initialize GPIOTE interrupt (will not be enabled until app_gpiote_user_enable() is called).
    NRF_GPIOTE->INTENCLR = 0xFFFFFFFF;

    NVIC_ClearPendingIRQ(GPIOTE_IRQn);
    NVIC_SetPriority(GPIOTE_IRQn, APP_IRQ_PRIORITY_LOW);
    NVIC_EnableIRQ(GPIOTE_IRQn);
}

void _GpioJoin(struct gpio_list *list)
{
	list_add_tail(&list->entries, &g_gpio_user);
	__gpio_sense_disable(list->user_info.pin_number);
}


static H_U32 __gpio_enable(H_U32 pin_number)
{
	struct gpio_list *pos;
	H_U32 pins_state = 0;
	struct gpio_manage* manage = __gpio_manage_get();
	
	NRF_GPIOTE->EVENTS_PORT = 0;
    pins_state = NRF_GPIO->IN;
    if(manage->gpio_mask == 0)
    {
    	NRF_GPIOTE->INTENSET = GPIOTE_INTENSET_PORT_Msk;
    }
    
	manage->gpio_mask |= (1 << pin_number); 
	list_for_each_entry(pos, &g_gpio_user, entries,struct gpio_list)
	{
		uint32_t pin_mask = (1 << pin_number);
		if(pos != H_NULL)
		{
			pos->user_info.sense_high_pins = 0;
			if((pos->user_info.pin_number == pin_number) && ((pos->user_info.pin_mask & (1 << pin_number)) != 0))
			{
				H_U32 sense = 0;
				H_U32 sense_set = (NRF_GPIO->PIN_CNF[pin_number])&GPIO_PIN_CNF_SENSE_Msk;
				if(sense_set & GPIO_PIN_CNF_SENSE_Msk)
				{
					sense = sense_set;
				}
				else
				{
					if((pins_state & pos->user_info.pin_mask) != 0)	
					{
						sense = GPIO_PIN_CNF_SENSE_Low << GPIO_PIN_CNF_SENSE_Pos;
					}
					else
					{
						sense = GPIO_PIN_CNF_SENSE_High << GPIO_PIN_CNF_SENSE_Pos;
						
					}
				}

				if(sense == (GPIO_PIN_CNF_SENSE_High << GPIO_PIN_CNF_SENSE_Pos))
				{
					pos->user_info.sense_high_pins |= pin_mask;
				}
				NRF_GPIO->PIN_CNF[pin_number] &= ~GPIO_PIN_CNF_SENSE_Msk;
            	NRF_GPIO->PIN_CNF[pin_number] |= sense;
            	return H_SUCCESS;
			}
		}
	}
	return H_FAULTER;
}

static H_U32 __gpio_sense_disable(H_U32 pin_number)
{
	struct gpio_list *pos;
	list_for_each_entry(pos, &g_gpio_user, entries,struct gpio_list)
	{
		if(pos != H_NULL)
		{
			if((pos->user_info.pin_number == pin_number) && ((pos->user_info.pin_mask & (1 << pin_number)) != 0))
			{
				NRF_GPIO->PIN_CNF[pin_number] &= ~GPIO_PIN_CNF_SENSE_Msk;
            	NRF_GPIO->PIN_CNF[pin_number] |= GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos;
            	return H_SUCCESS;
			}
		}
	}
	return H_SUCCESS; 
}

static H_U32 __gpio_disable(H_U32 pin_number)
{
	struct gpio_manage* manage = __gpio_manage_get();
	
	__gpio_sense_disable(pin_number);
	manage->gpio_mask &= ~(1UL << pin_number);
    if (manage->gpio_mask == 0)
    {
        NRF_GPIOTE->INTENCLR = GPIOTE_INTENSET_PORT_Msk;
    }
		return H_SUCCESS; 
}

H_U32 _GpioOnOff(H_U32 Pin, H_BOOL en)
{
	if(en)
	{
		return __gpio_enable(Pin);
	}else
	{
		return __gpio_disable(Pin);
	}
}

static H_U32 __gpio_sense_level_toggle(struct gpio_list *list, H_U32 pins_mask)
{
	H_U32 pin_no;
	for (pin_no = 0; pin_no < 32; pin_no++)
	{
		H_U32 pin_mask = (1 << pin_no);
		if ((pins_mask & pin_mask) != 0)
        {
            H_U32 sense;

            // Invert sensing.
            
            if ((list->user_info.sense_high_pins & pin_mask) == 0)
            {
                sense      = GPIO_PIN_CNF_SENSE_High << GPIO_PIN_CNF_SENSE_Pos;
                list->user_info.sense_high_pins |= pin_mask;
            }
            else
            {
                sense                    = GPIO_PIN_CNF_SENSE_Low << GPIO_PIN_CNF_SENSE_Pos;
                list->user_info.sense_high_pins &= ~pin_mask;
            }

            NRF_GPIO->PIN_CNF[pin_no] &= ~GPIO_PIN_CNF_SENSE_Msk;
            NRF_GPIO->PIN_CNF[pin_no] |= sense;
        }
	}
	
	return H_SUCCESS; 
}


void GPIOTE_IRQHandler(void)
{
	struct gpio_list *pos;
	struct gpio_manage* manage = __gpio_manage_get();
	H_U32 pins_state = NRF_GPIO->IN;
	H_U32 pins_changed = 0;

	//uint8_t nested_critical_region = 0;
	//APP_ERROR_CHECK(sd_nvic_critical_region_enter(&nested_critical_region)); //ÁÙ½çÇø
	NRF_GPIOTE->EVENTS_PORT = 0;
	list_for_each_entry(pos, &g_gpio_user, entries,struct gpio_list)
	{
		if(pos != H_NULL)
		{
			
			if((manage->gpio_mask & (1 << pos->user_info.pin_number)) != 0)
			{
				
				H_U32 transition_pins = 0;
				// Transition pins detected in late stage.
				H_U32 late_transition_pins = 0;
				H_U32 event_high_to_low = 0;
				H_U32 event_low_to_high = 0;

				// Find set of pins on which there has been a transition.
				transition_pins = (pins_state ^ ~(pos->user_info.sense_high_pins)) & pos->user_info.pin_mask;
				// Toggle SENSE level for all pins that have changed state.
				__gpio_sense_level_toggle(pos , transition_pins);

				// Second read after setting sense.
           	    // Check if any pins have changed while serving this interrupt.
				pins_changed = NRF_GPIO->IN ^ pins_state;
				
				if(pins_changed)
				{
					pins_state |= pins_changed;
					// Find set of pins on which there has been a transition.
					late_transition_pins = (pins_state ^ ~(pos->user_info.sense_high_pins)) & pos->user_info.pin_mask;

					// Toggle SENSE level for all pins that have changed state in last phase.
					__gpio_sense_level_toggle(pos, late_transition_pins);

					 // Update pins that has changed state since the interrupt occurred.
					transition_pins |= late_transition_pins;
				}

				// Call user event handler if an event has occurred.
            	event_high_to_low = (~pins_state & pos->user_info.pins_high_to_low_mask) & transition_pins;
            	event_low_to_high = (pins_state & pos->user_info.pins_low_to_high_mask) & transition_pins;
            	if((event_low_to_high | event_high_to_low) != 0)
            	{
					//disable the gpiote_irqn
					//NVIC_DisableIRQ(GPIOTE_IRQn);
					pos->user_info._callback(event_low_to_high, event_high_to_low);
					//re-enable the gpiote_irqn
					//NVIC_ClearPendingIRQ(GPIOTE_IRQn);
    				//NVIC_SetPriority(GPIOTE_IRQn, APP_IRQ_PRIORITY_LOW);
   					//NVIC_EnableIRQ(GPIOTE_IRQn);
            	}
			}
		}
	}
	//APP_ERROR_CHECK(sd_nvic_critical_region_exit(nested_critical_region));
}

