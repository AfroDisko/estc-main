#ifndef LEDS_H
#define LEDS_H

#include <stdint.h>

#include "nrfx_timer.h"

#include "common.h"

uint32_t ledsColorToPin(char color);

void ledsResetLEDsPins(void);

void ledsSetupLEDsGPIO(void);

void ledsSetLEDState(char color, LogicalState state);

LogicalState ledsGetLEDState(char color);

void ledsUpdateDutyCycle(void);

void ledsHandlerGenerator(nrf_timer_event_t event_type, void* p_context);

void ledsHandlerPeriodSmoothBlink(nrf_timer_event_t event_type, void* p_context);

void ledsSetupLEDsTimers(void);

void ledsSmoothBlink(char color);

bool ledsIsSmoothBlinking(void);

void ledsSmoothBlinkPause(void);

void ledsSmoothBlinkResume(void);

#endif
