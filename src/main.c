#include <string.h>

#include "nrf_delay.h"

#include "gpio2led.h"
#include "switch2gpio.h"

#define DELAY_BLINK 500

int main(void)
{
    switch2gpioSetupSwitch();
    gpio2ledSetupLEDs();

    const char patern[] = "YYYYYYYRGGGGGGGGGBBBBBBB";
    const int paternLength = strlen(patern);
    
    int currIdx = 0;

    while(true)
    {
        if(switch2gpioGetSwitchState() == LogicalStateON &&
           gpio2ledGetLEDState(patern[currIdx]) == LogicalStateOFF)
        {
            gpio2ledSetLEDState(patern[currIdx], LogicalStateON);
            nrf_delay_ms(DELAY_BLINK);
        }
        if(switch2gpioGetSwitchState() == LogicalStateON &&
           gpio2ledGetLEDState(patern[currIdx]) == LogicalStateON)
        {
            gpio2ledSetLEDState(patern[currIdx], LogicalStateOFF);
            ++currIdx;
            if(currIdx == paternLength)
                currIdx = 0;
            nrf_delay_ms(DELAY_BLINK);
        }
    }
}
