#include <stdbool.h>
#include <string.h>

#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_log_backend_usb.h"

#include "app_timer.h"
#include "nrfx_systick.h"

#include "switch.h"
#include "leds.h"
#include "queue.h"

int main(void)
{   
    NRF_LOG_INIT(NULL);
    NRF_LOG_DEFAULT_BACKENDS_INIT();

    nrfx_gpiote_init();

    app_timer_init();
    nrfx_systick_init();

    switchSetupGPIO();
    switchSetupGPIOTE();
    switchSetupTimers();

    ledsSetupGPIO();
    ledsSetupTimers();

    char gPattern[] = "gggggggRGGGGGGGGGBBBBBBB";
    unsigned int gCurrIdx = 0;

    ledsBlink(gPattern[gCurrIdx++]);

    while(true)
    {
        switch(queueEventDequeue())
        {
        case EventSwitchPressedSingle:
            break;
        case EventSwitchPressedDouble:
            if(ledsIsBlinking())
                ledsBlinkPause();
            else
                ledsBlinkResume();
            break;
        case EventSwitchPressedTriple:
            break;
        case EventLEDBlinkCompleted:
            ledsBlink(gPattern[gCurrIdx++]);
            if(gCurrIdx == strlen(gPattern))
                gCurrIdx = 0;
            break;
        default:
            break;
        }

        NRF_LOG_PROCESS();
        LOG_BACKEND_USB_PROCESS();
    }
}
