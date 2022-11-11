#ifndef LEDS_H
#define LEDS_H

#include <stdint.h>

#include "common.h"

uint32_t ledsColorToPin(char color);

void ledsResetPins(void);

void ledsSetupLEDs(void);

void ledsSetLEDState(char color, LogicalState state);

LogicalState ledsGetLEDState(char color);

#endif
