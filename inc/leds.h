#ifndef LEDS_H
#define LEDS_H

#include <stdbool.h>

void ledsSetupGPIO(void);

void ledsSetupTimers(void);

void ledsBlink(char color);

bool ledsIsBlinking(void);

void ledsBlinkPause(void);

void ledsBlinkResume(void);

#endif
