#include <stdbool.h>
#include <string.h>

#include "nrfx_gpiote.h"
#include "drv_rtc.h"
#include "app_timer.h"
#include "nrf_log_default_backends.h"

#include "gpio2led.h"
#include "switch2gpio.h"

void evtHandlerSW1(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    return;
}

int main(void)
{
    nrfx_gpiote_init();
    
    switch2gpioSetupSwitch();
    gpio2ledSetupLEDs();

    nrfx_gpiote_in_config_t configSW1 = NRFX_GPIOTE_CONFIG_IN_SENSE_TOGGLE(true);
    switch2gpioteSetupSwitch(&configSW1, evtHandlerSW1);
    switch2gpioteEventEnable(true);

    NRF_LOG_DEFAULT_BACKENDS_INIT();

    while(true)
    {
        
    }
}
