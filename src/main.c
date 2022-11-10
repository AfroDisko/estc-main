#include <stdbool.h>
#include <string.h>

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
    eventQueueEventEnqueue(switch2gpioGetSwitchState() == LogicalStateON ? SW1Pressed : SW1Released);
}

int main(void)
{   
    gpio2ledSetupLEDs();
    switch2gpioSetupSwitch();

    nrfx_gpiote_init();
    nrfx_gpiote_in_config_t configSW1 = NRFX_GPIOTE_CONFIG_IN_SENSE_TOGGLE(true);
    switch2gpioteSetupSwitch(&configSW1, evtHandlerSW1);
    switch2gpioteEventEnable(true);

    app_timer_init();
    app_timer_create(&switch2gpioTimerDebounce, APP_TIMER_MODE_SINGLE_SHOT, evtHandlerTimerDebounce);

    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
    NRF_LOG_DEFAULT_BACKENDS_INIT();

    while(true)
    {
        switch(eventQueueEventDequeue())
        {
        case DummyEvent:
            break;
        case SW1Pressed:
            gpio2ledSetLEDState('R', LogicalStateON);
            break;
        case SW1Released:
            gpio2ledSetLEDState('R', LogicalStateOFF);
            break;
        }

        NRF_LOG_PROCESS();
        LOG_BACKEND_USB_PROCESS();
    }
}
