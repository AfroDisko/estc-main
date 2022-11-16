#include <math.h>

#include "nrf_gpio.h"
#include "app_timer.h"
#include "nrfx_systick.h"

#include "leds.h"
#include "queue.h"

#define LED1_G_PRT 0
#define LED1_G_PIN 6

#define LED2_R_PRT 0
#define LED2_R_PIN 8

#define LED2_G_PRT 1
#define LED2_G_PIN 9

#define LED2_B_PRT 0
#define LED2_B_PIN 12

#define BLINK_PERIOD_MS     1000
#define GENERATOR_PERIOD_MS 1
#define CHECKER_PERIOD_US   10

nrfx_timer_t gTimerPeriod = NRFX_TIMER_INSTANCE(0);
nrfx_timer_config_t gTimerPeriodConfig = {
    NRF_TIMER_FREQ_31250Hz,
    NRF_TIMER_MODE_TIMER,
    NRF_TIMER_BIT_WIDTH_16,
    6,
    NULL
};

APP_TIMER_DEF(gTimerGenerator);

nrfx_timer_t gTimerChecker = NRFX_TIMER_INSTANCE(1);
nrfx_timer_config_t gTimerCheckerConfig = {
    NRF_TIMER_FREQ_1MHz,
    NRF_TIMER_MODE_TIMER,
    NRF_TIMER_BIT_WIDTH_16,
    6,
    NULL
};

nrfx_systick_state_t gSystickState;

volatile char gColor;

volatile double       gDutyCycle       = 0.;
volatile unsigned int gDutyCycleTick   = 0;
volatile bool         gDutyCycleUpdate = true;

uint32_t ledsColorToPin(char color)
{
    switch(color)
    {
    case 'g':
        return NRF_GPIO_PIN_MAP(LED1_G_PRT, LED1_G_PIN);
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
    nrf_gpio_pin_write(NRF_GPIO_PIN_MAP(LED1_G_PRT, LED1_G_PIN), 1);
    nrf_gpio_pin_write(NRF_GPIO_PIN_MAP(LED2_R_PRT, LED2_R_PIN), 1);
    nrf_gpio_pin_write(NRF_GPIO_PIN_MAP(LED2_G_PRT, LED2_G_PIN), 1);
    nrf_gpio_pin_write(NRF_GPIO_PIN_MAP(LED2_B_PRT, LED2_B_PIN), 1);
}

void ledsSetupGPIO(void)
{
    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(LED1_G_PRT, LED1_G_PIN));
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

// cos() is used for automatic periodic behavior of the duty cycle
void ledsUpdateDutyCycle(void)
{
    gDutyCycle = 0.5 - 0.5 * cos(2 * 3.1415 * gDutyCycleTick++ * GENERATOR_PERIOD_MS / BLINK_PERIOD_MS);
}

void ledsHandlerPeriod(nrf_timer_event_t event_type, void* p_context)
{
    switch(event_type)
    {
    case NRF_TIMER_EVENT_COMPARE0:
        nrfx_timer_disable(&gTimerPeriod);
        app_timer_stop(gTimerGenerator);
        nrfx_timer_disable(&gTimerChecker);
        ledsSetLEDState(gColor, LogicalStateOff);
        queueEventEnqueue(EventLEDBlinkCompleted);
        break;
    default:
        break;
    }
}

void ledsHandlerGenerator(void* p_context)
{
    if(gDutyCycleUpdate)
        ledsUpdateDutyCycle();
    ledsSetLEDState(gColor, LogicalStateOn);
    nrfx_systick_get(&gSystickState);
    nrfx_timer_enable(&gTimerChecker);
}

void ledsHandlerChecker(nrf_timer_event_t event_type, void* p_context)
{
    switch(event_type)
    {
    case NRF_TIMER_EVENT_COMPARE0:
        if(nrfx_systick_test(&gSystickState, gDutyCycle * GENERATOR_PERIOD_MS * 1000))
            ledsSetLEDState(gColor, LogicalStateOff);
        break;
    default:
        break;
    }
}

void ledsSetupTimers(void)
{
    nrfx_timer_init(&gTimerPeriod, &gTimerPeriodConfig, ledsHandlerPeriod);
    nrfx_timer_extended_compare(&gTimerPeriod, NRF_TIMER_CC_CHANNEL0,
                                nrfx_timer_ms_to_ticks(&gTimerPeriod, BLINK_PERIOD_MS),
                                NRF_TIMER_SHORT_COMPARE0_STOP_MASK, true);

    app_timer_create(&gTimerGenerator, APP_TIMER_MODE_REPEATED, ledsHandlerGenerator);

    nrfx_timer_init(&gTimerChecker, &gTimerCheckerConfig, ledsHandlerChecker);
    nrfx_timer_extended_compare(&gTimerChecker, NRF_TIMER_CC_CHANNEL0,
                                nrfx_timer_us_to_ticks(&gTimerChecker, CHECKER_PERIOD_US),
                                NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);
}

void ledsBlink(char color)
{
    gColor = color;
    gDutyCycle = 0.;
    gDutyCycleTick = 0;
    gDutyCycleUpdate = true;
    nrfx_timer_enable(&gTimerPeriod);
    app_timer_start(gTimerGenerator, APP_TIMER_TICKS(GENERATOR_PERIOD_MS), NULL);
}

bool ledsIsBlinking(void)
{
    return gDutyCycleUpdate;
}

void ledsBlinkPause(void)
{
    gDutyCycleUpdate = false;
    nrfx_timer_pause(&gTimerPeriod);
}

void ledsBlinkResume(void)
{
    gDutyCycleUpdate = true;
    nrfx_timer_resume(&gTimerPeriod);
}
