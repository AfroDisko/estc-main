#include <string.h>

#include "nrf_delay.h"

#include "gpio2led.h"
#include "switch2gpio.h"

#define DELAY_BLINK 1000

int main(void)
{
    switch2gpioSetupSwitch();
    gpio2ledSetupLEDs();

    const char patern[] = "YRGB";
    const int paternLength = strlen(patern);
    
    int currIdx = 0;

    while(true)
    {
        if(switch2gpioReadSwitchState())
        {
            gpio2ledSwitchLED(patern[currIdx], gpio2led_ON);
            nrf_delay_ms(DELAY_BLINK);
        }
        if(switch2gpioReadSwitchState())
        {
            gpio2ledSwitchLED(patern[currIdx], gpio2led_OFF);
            ++currIdx;
            if(currIdx == paternLength)
                currIdx = 0;
            nrf_delay_ms(DELAY_BLINK);
        }
    }
}
