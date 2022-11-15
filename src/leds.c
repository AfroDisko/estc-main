#include <math.h>

#include "nrf_gpio.h"
#include "nrfx_systick.h"

#include "leds.h"

#define LED1_Y_PRT 0
#define LED1_Y_PIN 6

#define LED2_R_PRT 0
#define LED2_R_PIN 8

#define LED2_G_PRT 1
#define LED2_G_PIN 9

#define LED2_B_PRT 0
#define LED2_B_PIN 12

#define GENERATOR_PERIOD_MS 1
#define LED_BLINK_PERIOD_MS 1000

nrfx_timer_t gTimerGenerator = NRFX_TIMER_INSTANCE(3);
nrfx_timer_config_t gTimerGeneratorConfig = NRFX_TIMER_DEFAULT_CONFIG;

nrfx_timer_t gTimerPeriodSmoothBlink = NRFX_TIMER_INSTANCE(4);
nrfx_timer_config_t gTimerPeriodSmoothBlinkConfig = NRFX_TIMER_DEFAULT_CONFIG;

volatile char gLEDColor;

volatile float        gDutyCycle;
volatile unsigned int gDutyCycleTick;
volatile bool         gDutyCycleUpdate;

uint32_t ledsColorToPin(char color)
{
    switch (color)
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

void ledsResetLEDsPins(void)
{
    nrf_gpio_pin_write(NRF_GPIO_PIN_MAP(LED1_Y_PRT, LED1_Y_PIN), 1);
    nrf_gpio_pin_write(NRF_GPIO_PIN_MAP(LED2_R_PRT, LED2_R_PIN), 1);
    nrf_gpio_pin_write(NRF_GPIO_PIN_MAP(LED2_G_PRT, LED2_G_PIN), 1);
    nrf_gpio_pin_write(NRF_GPIO_PIN_MAP(LED2_B_PRT, LED2_B_PIN), 1);
}

void ledsSetupLEDsGPIO(void)
{
    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(LED1_Y_PRT, LED1_Y_PIN));
    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(LED2_R_PRT, LED2_R_PIN));
    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(LED2_G_PRT, LED2_G_PIN));
    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(LED2_B_PRT, LED2_B_PIN));

    ledsResetLEDsPins();
}

void ledsSetLEDState(char color, LogicalState state)
{
    nrf_gpio_pin_write(ledsColorToPin(color), state == LogicalStateOn ? 0 : 1);
}

LogicalState ledsGetLEDState(char color)
{
    return nrf_gpio_pin_out_read(ledsColorToPin(color)) == 0 ? LogicalStateOn : LogicalStateOff;
}

void ledsUpdateDutyCycle(void)
{
    gDutyCycle = 0.5 - 0.5 * cos(2 * 3.14 * gDutyCycleTick++ * GENERATOR_PERIOD_MS / LED_BLINK_PERIOD_MS);
}

void ledsHandlerGenerator(nrf_timer_event_t event_type, void* p_context)
{
    switch(event_type)
    {
    case NRF_TIMER_EVENT_COMPARE0:
        ledsSetLEDState(gLEDColor, LogicalStateOn);
        nrfx_systick_delay_us(gDutyCycle * GENERATOR_PERIOD_MS * 1000);
        ledsSetLEDState(gLEDColor, LogicalStateOff);
        if(gDutyCycleUpdate)
            ledsUpdateDutyCycle();
        break;
    default:
        break;
    }
}

void ledsHandlerPeriodSmoothBlink(nrf_timer_event_t event_type, void* p_context)
{
    switch(event_type)
    {
    case NRF_TIMER_EVENT_COMPARE0:
        nrfx_timer_disable(&gTimerGenerator);
        nrfx_timer_disable(&gTimerPeriodSmoothBlink);
        ledsSetLEDState(gLEDColor, LogicalStateOff);
        break;
    default:
        break;
    }
}

void ledsSetupLEDsTimers(void)
{
    nrfx_timer_init(&gTimerGenerator, &gTimerGeneratorConfig, ledsHandlerGenerator);
    nrfx_timer_extended_compare(&gTimerGenerator, NRF_TIMER_CC_CHANNEL0,
                                nrfx_timer_ms_to_ticks(&gTimerGenerator, GENERATOR_PERIOD_MS),
                                NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);

    nrfx_timer_init(&gTimerPeriodSmoothBlink, &gTimerPeriodSmoothBlinkConfig, ledsHandlerPeriodSmoothBlink);
    nrfx_timer_extended_compare(&gTimerPeriodSmoothBlink, NRF_TIMER_CC_CHANNEL0,
                                nrfx_timer_ms_to_ticks(&gTimerPeriodSmoothBlink, LED_BLINK_PERIOD_MS),
                                NRF_TIMER_SHORT_COMPARE0_STOP_MASK, true);
}

void ledsSmoothBlink(char color)
{
    gLEDColor = color;
    gDutyCycleTick = 0;
    gDutyCycleUpdate = true;
    nrfx_timer_enable(&gTimerGenerator);
    nrfx_timer_enable(&gTimerPeriodSmoothBlink);
}

bool ledsIsSmoothBlinking(void)
{
    return gDutyCycleUpdate;
}

void ledsSmoothBlinkPause(void)
{
    gDutyCycleUpdate = false;
    nrfx_timer_pause(&gTimerPeriodSmoothBlink);
}

void ledsSmoothBlinkResume(void)
{
    gDutyCycleUpdate = true;
    nrfx_timer_resume(&gTimerPeriodSmoothBlink);
}
