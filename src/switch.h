#ifndef SWITCH_H
#define SWITCH_H

#include "nrfx_gpiote.h"

#include "common.h"

LogicalState switchGetSwitchState(void);

void switchSetupSwitchEvent(nrfx_gpiote_in_config_t* config, nrfx_gpiote_evt_handler_t handler);

void switchEnableEvent(bool int_enable);

void switchSetupSwitch(void);

void switchExpectPressDouble(void);

void switchHandlerSW1(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action);

void switchHandlerDebounce();

void switchHandlerPressDouble();

#endif
