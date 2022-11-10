#ifndef SWITCH2GPIO_H
#define SWITCH2GPIO_H

#include "nrfx_gpiote.h"

#include "common.h"

void switch2gpioSetupSwitch(void);

LogicalState switch2gpioGetSwitchState(void);

void switch2gpioteSetupSwitch(nrfx_gpiote_in_config_t* config, nrfx_gpiote_evt_handler_t handler);

void switch2gpioteEventEnable(bool int_enable);

#endif
