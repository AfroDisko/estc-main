#include <stdbool.h>
#include <string.h>

#include "nrfx_gpiote.h"

#include "switch2gpio.h"
#include "gpio2led.h"

#define SW1_PRT 1
#define SW1_PIN 6

volatile bool gEvtFlagSW1InProgress = false;
volatile bool gEvtFlagSW1State      = false;

void evtHandlerSW1(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    gFlagSW1State = switch2gpioGetSwitchState() == LogicalStateON;
}

int main(void)
{
    nrfx_gpiote_init();

    nrfx_gpiote_in_config_t sw1Config = NRFX_GPIOTE_CONFIG_IN_SENSE_TOGGLE(true);
    sw1Config.pull = NRF_GPIO_PIN_PULLUP;

    nrfx_gpiote_in_init(NRF_GPIO_PIN_MAP(SW1_PRT, SW1_PIN), &sw1Config, evtHandlerSW1);

    nrfx_gpiote_in_event_enable(NRF_GPIO_PIN_MAP(SW1_PRT, SW1_PIN), true);

    switch2gpioSetupSwitch();
    gpio2ledSetupLEDs();

    gpio2ledSetLEDState('Y', LogicalStateON);

    while(true)
    {
        gpio2ledSetLEDState('R', gFlagSW1State ? LogicalStateON : LogicalStateOFF);
    }
}
