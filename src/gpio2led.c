#include "gpio2led.h"

void gpio2ledResetPins()
{
    nrf_gpio_pin_write(NRF_GPIO_PIN_MAP(LED1_prt, LED1_pin), 1);
    nrf_gpio_pin_write(NRF_GPIO_PIN_MAP(LED2_R_prt, LED2_R_pin), 1);
    nrf_gpio_pin_write(NRF_GPIO_PIN_MAP(LED2_G_prt, LED2_G_pin), 1);
    nrf_gpio_pin_write(NRF_GPIO_PIN_MAP(LED2_B_prt, LED2_B_pin), 1);
}

void gpio2ledSetupLEDs()
{
    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(LED1_prt, LED1_pin));
    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(LED2_R_prt, LED2_R_pin));
    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(LED2_G_prt, LED2_G_pin));
    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(LED2_B_prt, LED2_B_pin));

    gpio2ledResetPins();
}

uint32_t color2pin(char color)
{
    switch(color)
    {
    case 'Y':
        return NRF_GPIO_PIN_MAP(LED1_prt, LED1_pin);
    case 'R':
        return NRF_GPIO_PIN_MAP(LED2_R_prt, LED2_R_pin);
    case 'G':
        return NRF_GPIO_PIN_MAP(LED2_G_prt, LED2_G_pin);
    case 'B':
        return NRF_GPIO_PIN_MAP(LED2_B_prt, LED2_B_pin);
    default:
        return 0;
    }
}

void gpio2ledSwitchLED(char color, gpio2ledState state)
{
    nrf_gpio_pin_write(color2pin(color), state == gpio2led_ON ? 0 : 1);
}

gpio2ledState gpio2ledReadLEDState(char color)
{
    return nrf_gpio_pin_out_read(color2pin(color)) == 0 ? gpio2led_ON : gpio2led_OFF;
}
