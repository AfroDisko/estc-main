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

#define PWM_GENERATOR_PERIOD_MS 1
#define BLINK_PERIOD_MS 1000

nrfx_timer_t gTimerPWMGenerator = NRFX_TIMER_INSTANCE(3);
nrfx_timer_config_t gTimerPWMGeneratorConfig = NRFX_TIMER_DEFAULT_CONFIG;

nrfx_timer_t gTimerBlinkPeriod = NRFX_TIMER_INSTANCE(4);
nrfx_timer_config_t gTimerBlinkPeriodConfig = NRFX_TIMER_DEFAULT_CONFIG;

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
    gDutyCycle = 0.5 - 0.5 * cos(2 * 3.14 * gDutyCycleTick++ * PWM_GENERATOR_PERIOD_MS / BLINK_PERIOD_MS);
}

void ledsHandlerPWMGenerator(nrf_timer_event_t event_type, void* p_context)
{
    switch(event_type)
    {
    case NRF_TIMER_EVENT_COMPARE0:
        ledsSetLEDState(gLEDColor, LogicalStateOn);
        nrfx_systick_delay_us(gDutyCycle * PWM_GENERATOR_PERIOD_MS * 1000);
        ledsSetLEDState(gLEDColor, LogicalStateOff);
        if(gDutyCycleUpdate)
            ledsUpdateDutyCycle();
        break;
    default:
        break;
    }
}

void ledsHandlerBlinkPeriod(nrf_timer_event_t event_type, void* p_context)
{
    switch(event_type)
    {
    case NRF_TIMER_EVENT_COMPARE0:
        nrfx_timer_disable(&gTimerPWMGenerator);
        nrfx_timer_disable(&gTimerBlinkPeriod);
        ledsSetLEDState(gLEDColor, LogicalStateOff);
        break;
    default:
        break;
    }
}

void ledsSetupLEDsTimers(void)
{
    nrfx_timer_init(&gTimerPWMGenerator, &gTimerPWMGeneratorConfig, ledsHandlerPWMGenerator);
    nrfx_timer_extended_compare(&gTimerPWMGenerator, NRF_TIMER_CC_CHANNEL0,
                                nrfx_timer_ms_to_ticks(&gTimerPWMGenerator, PWM_GENERATOR_PERIOD_MS),
                                NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);

    nrfx_timer_init(&gTimerBlinkPeriod, &gTimerBlinkPeriodConfig, ledsHandlerBlinkPeriod);
    nrfx_timer_extended_compare(&gTimerBlinkPeriod, NRF_TIMER_CC_CHANNEL0,
                                nrfx_timer_ms_to_ticks(&gTimerBlinkPeriod, BLINK_PERIOD_MS),
                                NRF_TIMER_SHORT_COMPARE0_STOP_MASK, true);
}

void ledsBlink(char color)
{
    gLEDColor = color;
    gDutyCycleTick = 0;
    gDutyCycleUpdate = true;
    nrfx_timer_enable(&gTimerPWMGenerator);
    nrfx_timer_enable(&gTimerBlinkPeriod);
}

bool ledsIsBlinking(void)
{
    return gDutyCycleUpdate;
}

void ledsBlinkPause(void)
{
    gDutyCycleUpdate = false;
    nrfx_timer_pause(&gTimerBlinkPeriod);
}

void ledsBlinkResume(void)
{
    gDutyCycleUpdate = true;
    nrfx_timer_resume(&gTimerBlinkPeriod);
}
