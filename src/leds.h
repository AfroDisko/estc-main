#ifndef LEDS_H
#define LEDS_H

#include <stdint.h>

#include "nrfx_timer.h"

#include "common.h"

void ledsSetLEDState(char color, LogicalState state);

LogicalState ledsGetLEDState(char color);

uint32_t ledsColorToPin(char color);

void ledsResetPins(void);

void ledsSetupLEDs(void);

void ledsFlashingStart(char color);

void ledsFlashingStop(char color);

void ledsHandlerGenerator(nrf_timer_event_t event_type, void* p_context);

void ledsHandlerDuty(nrf_timer_event_t event_type, void* p_context);

#endif
