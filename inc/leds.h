#ifndef LEDS_H
#define LEDS_H

#include <stdint.h>

#include "common.h"
#include "ledsutils.h"

typedef enum
{
    FlashModeSlow,
    FlashModeFast
} FlashMode;

void ledsSetupGPIO(void);

void ledsSetLEDState(char color, LogicalState state);

LogicalState ledsGetLEDState(char color);

void ledsSetupPWM(void);

void ledsSetLED1State(uint8_t state);

void ledsSetLED2State(ColorHSV hsv);

void ledsSetupLED1Timer(void);

void ledsFlashLED1(FlashMode mode);

void ledsFlashLED1Halt(void);

#endif
