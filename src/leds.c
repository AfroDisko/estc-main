#include "nrf_gpio.h"

#include "leds.h"

#define LED1_Y_PRT 0
#define LED1_Y_PIN 6

#define LED2_R_PRT 0
#define LED2_R_PIN 8

#define LED2_G_PRT 1
#define LED2_G_PIN 9

#define LED2_B_PRT 0
#define LED2_B_PIN 12

uint32_t ledsColorToPin(char color)
{
    switch(color)
    {
    case 'Y':
        return NRF_GPIO_PIN_MAP(LED1_Y_PRT, LED1_Y_PIN);
    case 'R':
        return NRF_GPIO_PIN_MAP(LED2_R_PRT, LED2_R_PIN);
    case 'G':
        return NRF_GPIO_PIN_MAP(LED2_G_PRT, LED2_G_PIN);
    case 'B':
        return NRF_GPIO_PIN_MAP(LED2_B_PRT, LED2_B_PIN);
    default:
        return 0;
    }
}

void ledsResetPins(void)
{
    nrf_gpio_pin_write(NRF_GPIO_PIN_MAP(LED1_Y_PRT, LED1_Y_PIN), 1);
    nrf_gpio_pin_write(NRF_GPIO_PIN_MAP(LED2_R_PRT, LED2_R_PIN), 1);
    nrf_gpio_pin_write(NRF_GPIO_PIN_MAP(LED2_G_PRT, LED2_G_PIN), 1);
    nrf_gpio_pin_write(NRF_GPIO_PIN_MAP(LED2_B_PRT, LED2_B_PIN), 1);
}

void ledsSetupLEDs(void)
{
    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(LED1_Y_PRT, LED1_Y_PIN));
    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(LED2_R_PRT, LED2_R_PIN));
    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(LED2_G_PRT, LED2_G_PIN));
    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(LED2_B_PRT, LED2_B_PIN));

    ledsResetPins();
}

void ledsSetLEDState(char color, LogicalState state)
{
    nrf_gpio_pin_write(ledsColorToPin(color), state == LogicalStateOn ? 0 : 1);
}

LogicalState ledsGetLEDState(char color)
{
    return nrf_gpio_pin_out_read(ledsColorToPin(color)) == 0 ? LogicalStateOn : LogicalStateOff;
}
