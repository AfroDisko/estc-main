#ifndef GPIO2LED_H
#define GPIO2LED_H

#include <stdint.h>

#include "common.h"

uint32_t color2pin(char color);

void gpio2ledResetPins(void);

void gpio2ledSetupLEDs(void);

void gpio2ledSetLEDState(char color, LogicalState state);

LogicalState gpio2ledGetLEDState(char color);

#endif
