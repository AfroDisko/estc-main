#include "nrf_gpio.h"
#include "switch2gpio.h"

#define SW1_PRT 1
#define SW1_PIN 6

void switch2gpioSetupSwitch(void)
{
    nrf_gpio_cfg_input(NRF_GPIO_PIN_MAP(SW1_PRT, SW1_PIN), NRF_GPIO_PIN_PULLUP);
}

LogicalState switch2gpioGetSwitchState(void)
{
    return nrf_gpio_pin_read(NRF_GPIO_PIN_MAP(SW1_PRT, SW1_PIN)) == 0 ? LogicalStateON : LogicalStateOFF;
}

void switch2gpioteSetupSwitch(nrfx_gpiote_in_config_t* config, nrfx_gpiote_evt_handler_t handler)
{
    config->pull = NRF_GPIO_PIN_PULLUP;
    nrfx_gpiote_in_init(NRF_GPIO_PIN_MAP(SW1_PRT, SW1_PIN), config, handler);
}

void switch2gpioteEventEnable(bool int_enable)
{
    nrfx_gpiote_in_event_enable(NRF_GPIO_PIN_MAP(SW1_PRT, SW1_PIN), int_enable);
}
