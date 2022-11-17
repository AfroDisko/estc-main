#ifndef LEDS_H
#define LEDS_H

#include <stdint.h>

void ledsSetupGPIO(void);

void ledsSetupPWM(void);

void ledsSetLED2Sat(uint8_t s);

#endif
