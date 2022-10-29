#ifndef SWITCH2GPIO_H
#define SWITCH2GPIO_H

#include <stdbool.h>

#include "nrf_gpio.h"

#define SW1_prt 1
#define SW1_pin 6

void switch2gpioSetupSwitch();

bool switch2gpioReadSwitchState();

#endif
