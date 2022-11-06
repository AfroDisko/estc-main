#include <stdbool.h>
#include <string.h>

#include "app_timer.h"
#include "drv_rtc.h"

#include "gpio2led.h"
#include "switch2gpio.h"
#include "switch2gpiote.h"

volatile bool gFlagSW1InProgress = false;
volatile bool gFlagSW1State      = false;

void evtHandlerSW1(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    if(!gFlagSW1InProgress && switch2gpioGetSwitchState() == LogicalStateON)
    {
        gFlagSW1InProgress = true;
    }
}

int main(void)
{
    app_timer_init();
    nrfx_gpiote_init();
    switch2gpioSetupSwitch();
    gpio2ledSetupLEDs();

    nrfx_gpiote_in_config_t configSW1 = NRFX_GPIOTE_CONFIG_IN_SENSE_TOGGLE(true);
    configSW1.pull = NRF_GPIO_PIN_PULLUP;

    switch2gpioteSetupSwitch(&configSW1, evtHandlerSW1);
    switch2gpioteEventEnable(true);

    gpio2ledSetLEDState('Y', LogicalStateON);

    while(true)
    {
        gpio2ledSetLEDState('R', gFlagSW1State ? LogicalStateON : LogicalStateOFF);
    }
}
