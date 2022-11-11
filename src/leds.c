#include <math.h>

#include "app_timer.h"
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

#define LEDS_GENERATOR_PERIOD_US 1000
#define LEDS_LED_FLASH_PERIOD_US 500000

nrfx_timer_t ledsTimerGenerator = NRFX_TIMER_INSTANCE(3);
nrfx_timer_config_t ledsTimerGeneratorConfig = NRFX_TIMER_DEFAULT_CONFIG;

nrfx_timer_t ledsTimerDuty = NRFX_TIMER_INSTANCE(4);
nrfx_timer_config_t ledsTimerDutyConfig = NRFX_TIMER_DEFAULT_CONFIG;

volatile char gColor;
volatile float gDutyCycle = 1.f;

void ledsSetLEDState(char color, LogicalState state)
{
    nrf_gpio_pin_write(ledsColorToPin(color), state == LogicalStateOn ? 0 : 1);
}

LogicalState ledsGetLEDState(char color)
{
    return nrf_gpio_pin_out_read(ledsColorToPin(color)) == 0 ? LogicalStateOn : LogicalStateOff;
}

void ledsSetDutyCycle(float dutyCyle)
{
    gDutyCycle = dutyCyle;
}

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

    nrfx_timer_init(&ledsTimerGenerator, &ledsTimerGeneratorConfig, ledsHandlerGenerator);
    uint32_t dutyTicks = nrfx_timer_us_to_ticks(&ledsTimerGenerator, LEDS_GENERATOR_PERIOD_US);
    nrfx_timer_extended_compare(&ledsTimerGenerator, NRF_TIMER_CC_CHANNEL0, dutyTicks, NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);
    
    nrfx_timer_init(&ledsTimerDuty, &ledsTimerDutyConfig, ledsHandlerDuty);
}

void ledsFlashingStart(char color)
{
    gColor = color;
    nrfx_timer_enable(&ledsTimerGenerator);
}

void ledsFlashingStop(void)
{
    nrfx_timer_disable(&ledsTimerGenerator);
    nrfx_timer_disable(&ledsTimerDuty);
    ledsSetLEDState(gColor, LogicalStateOff);
}

void ledsHandlerGenerator(nrf_timer_event_t event_type, void* p_context)
{
    switch(event_type)
    {
    case NRF_TIMER_EVENT_COMPARE0:
        uint32_t dutyTicks = gDutyCycle * nrfx_timer_us_to_ticks(&ledsTimerDuty, LEDS_GENERATOR_PERIOD_US);
        nrfx_timer_extended_compare(&ledsTimerDuty, NRF_TIMER_CC_CHANNEL0, dutyTicks, NRF_TIMER_SHORT_COMPARE0_STOP_MASK, true);
        ledsSetLEDState(gColor, LogicalStateOn);
        nrfx_timer_enable(&ledsTimerDuty);
        break;
    default:
        break;
    }
}

void ledsHandlerDuty(nrf_timer_event_t event_type, void* p_context)
{
    switch(event_type)
    {
    case NRF_TIMER_EVENT_COMPARE0:
        ledsSetLEDState(gColor, LogicalStateOff);
        nrfx_timer_clear(&ledsTimerDuty);
        break;
    default:
        break;
    }
}
