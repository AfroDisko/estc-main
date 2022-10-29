#include "switch2gpio.h"

void switch2gpioSetupSwitch()
{
    nrf_gpio_cfg_input(NRF_GPIO_PIN_MAP(SW1_prt, SW1_pin), GPIO_PIN_CNF_PULL_Pullup);
}

bool switch2gpioReadSwitchState()
{
    return nrf_gpio_pin_read(NRF_GPIO_PIN_MAP(SW1_prt, SW1_pin)) == 0;
}
