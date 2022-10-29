#ifndef GPIO2LED_H
#define GPIO2LED_H

#include "nrf_gpio.h"

#define LED1_prt   0
#define LED1_pin   6

#define LED2_R_prt 0
#define LED2_R_pin 8

#define LED2_G_prt 1
#define LED2_G_pin 9

#define LED2_B_prt 0
#define LED2_B_pin 12

typedef enum {gpio2led_OFF, gpio2led_ON} gpio2ledState;

void gpio2ledResetPins();

void gpio2ledSetupLEDs();

uint32_t color2pin(char color);

void gpio2ledSwitchLED(char color, gpio2ledState state);

#endif
