#include "nrf_gpio.h"
#include "switch2gpio.h"

#define SW1_PRT 1
#define SW1_PIN 6

void switch2gpioSetupSwitch()
{
    nrf_gpio_cfg_input(NRF_GPIO_PIN_MAP(SW1_PRT, SW1_PIN), NRF_GPIO_PIN_PULLUP);
}

LogicalState switch2gpioGetSwitchState()
{
    return nrf_gpio_pin_read(NRF_GPIO_PIN_MAP(SW1_PRT, SW1_PIN)) == 0 ? LogicalStateON : LogicalStateOFF;
}
