#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_log_backend_usb.h"

#include "nrfx_gpiote.h"

#include "common.h"
#include "gpio2led.h"
#include "switch2gpio.h"
#include "eventQueue.h"

void evtHandlerSW1(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    app_timer_start(switch2gpioTimerDebounce, SWITCH2GPIO_DEBOUNCE_TICKS, NULL);       
}

void evtHandlerTimerDebounce()
{
    eventQueueEventEnqueue(switch2gpioGetSwitchState() == LogicalStateON ? SW1PressedSingle : SW1Released);
}

void evtHandlerTimerDouble()
{
    if(switch2gpioGetSwitchState() == LogicalStateON)
        eventQueueEventEnqueue(SW1PressedDouble);
}

void evtHandlerTimerGenerator()
{
    gpio2ledSetLEDState('R', LogicalStateON);
    app_timer_start(gpio2ledTimerDuty, APP_TIMER_TICKS(0.1), NULL);
}

void evtHandlerTimerDuty()
{
    gpio2ledSetLEDState('R', LogicalStateOFF);
}

int main(void)
{   
    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
    NRF_LOG_DEFAULT_BACKENDS_INIT();

    gpio2ledSetupLEDs();
    switch2gpioSetupSwitch();

    nrfx_gpiote_init();
    nrfx_gpiote_in_config_t configSW1 = NRFX_GPIOTE_CONFIG_IN_SENSE_TOGGLE(true);
    switch2gpioteSetupSwitch(&configSW1, evtHandlerSW1);
    switch2gpioteEventEnable(true);

    app_timer_init();
    app_timer_create(&switch2gpioTimerDebounce, APP_TIMER_MODE_SINGLE_SHOT, evtHandlerTimerDebounce);
    app_timer_create(&switch2gpioTimerDouble, APP_TIMER_MODE_SINGLE_SHOT, evtHandlerTimerDouble);
    app_timer_create(&gpio2ledTimerGenerator, APP_TIMER_MODE_REPEATED, evtHandlerTimerGenerator);
    app_timer_create(&gpio2ledTimerDuty, APP_TIMER_MODE_SINGLE_SHOT, evtHandlerTimerDuty);

    app_timer_start(gpio2ledTimerGenerator, GPIO2LED_GENERATOR_TICKS, NULL);

    while(true)
    {
        switch(eventQueueEventDequeue())
        {
        case DummyEvent:
            break;
        case SW1PressedSingle:
            break;
        case SW1PressedDouble:
            gpio2ledSetLEDState('Y', LogicalStateON);
            break;
        case SW1Released:
            gpio2ledSetLEDState('Y', LogicalStateOFF);
            app_timer_start(switch2gpioTimerDouble, 2 * SWITCH2GPIO_DEBOUNCE_TICKS, NULL);
            break;
        }

        NRF_LOG_PROCESS();
        LOG_BACKEND_USB_PROCESS();
    }
}
