#ifndef SWITCH2GPIO_H
#define SWITCH2GPIO_H

#include "app_timer.h"
#include "nrfx_gpiote.h"

#include "common.h"

#define SWITCH2GPIO_DEBOUNCE_TICKS APP_TIMER_TICKS(50)

APP_TIMER_DEF(switch2gpioTimerDebounce);

void switch2gpioSetupSwitch(void);

LogicalState switch2gpioGetSwitchState(void);

void switch2gpioteSetupSwitch(nrfx_gpiote_in_config_t* config, nrfx_gpiote_evt_handler_t handler);

void switch2gpioteEventEnable(bool int_enable);

#endif
