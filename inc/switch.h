#ifndef SWITCH_H
#define SWITCH_H

#include "common.h"

void switchSetupGPIO(void);

// LogicalState switchGetSwitchState(void);

// void switchHandlerSW1(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action);

void switchSetupGPIOTE(void);

// void switchHandlerListener(void* p_context);

// void switchHandlerDebounce(void* p_context);

void switchSetupTimers(void);

#endif
