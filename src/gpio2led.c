#include "nrf_gpio.h"
#include "gpio2led.h"

#define LED1_Y_PRT 0
#define LED1_Y_PIN 6

#define LED2_R_PRT 0
#define LED2_R_PIN 8

#define LED2_G_PRT 1
#define LED2_G_PIN 9

#define LED2_B_PRT 0
#define LED2_B_PIN 12

uint32_t color2pin(char color)
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

void gpio2ledResetPins()
{
    nrf_gpio_pin_write(NRF_GPIO_PIN_MAP(LED1_Y_PRT, LED1_Y_PIN), 1);
    nrf_gpio_pin_write(NRF_GPIO_PIN_MAP(LED2_R_PRT, LED2_R_PIN), 1);
    nrf_gpio_pin_write(NRF_GPIO_PIN_MAP(LED2_G_PRT, LED2_G_PIN), 1);
    nrf_gpio_pin_write(NRF_GPIO_PIN_MAP(LED2_B_PRT, LED2_B_PIN), 1);
}

void gpio2ledSetupLEDs()
{
    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(LED1_Y_PRT, LED1_Y_PIN));
    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(LED2_R_PRT, LED2_R_PIN));
    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(LED2_G_PRT, LED2_G_PIN));
    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(LED2_B_PRT, LED2_B_PIN));

    gpio2ledResetPins();
}

void gpio2ledSetLEDState(char color, LogicalState state)
{
    nrf_gpio_pin_write(color2pin(color), state == LogicalStateON ? 0 : 1);
}

LogicalState gpio2ledGetLEDState(char color)
{
    return nrf_gpio_pin_out_read(color2pin(color)) == 0 ? LogicalStateON : LogicalStateOFF;
}
