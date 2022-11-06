#include "switch2gpiote.h"

#define SW1_PRT 1
#define SW1_PIN 6

void switch2gpioteSetupSwitch(nrfx_gpiote_in_config_t* config, nrfx_gpiote_evt_handler_t handler)
{
    config->pull = NRF_GPIO_PIN_PULLUP;
    nrfx_gpiote_in_init(NRF_GPIO_PIN_MAP(SW1_PRT, SW1_PIN), config, handler);
}

void switch2gpioteEventEnable(bool int_enable)
{
    nrfx_gpiote_in_event_enable(NRF_GPIO_PIN_MAP(SW1_PRT, SW1_PIN), int_enable);
}
