#ifndef GPIO2LED_H
#define GPIO2LED_H

#include <stdint.h>

#include "app_timer.h"

#include "common.h"

#define GPIO2LED_GENERATOR_TICKS APP_TIMER_TICKS(1)

APP_TIMER_DEF(gpio2ledTimerGenerator);

APP_TIMER_DEF(gpio2ledTimerDuty);

uint32_t color2pin(char color);

void gpio2ledResetPins(void);

void gpio2ledSetupLEDs(void);

void gpio2ledSetLEDState(char color, LogicalState state);

LogicalState gpio2ledGetLEDState(char color);

#endif
