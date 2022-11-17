#include <stdbool.h>
#include <string.h>

#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_log_backend_usb.h"
#include "nrfx_gpiote.h"
#include "app_timer.h"

#include "switch.h"
#include "leds.h"
#include "queue.h"

int main(void)
{   
    NRF_LOG_INIT(NULL);
    NRF_LOG_DEFAULT_BACKENDS_INIT();

    nrfx_gpiote_init();
    app_timer_init();

    switchSetupGPIO();
    switchSetupGPIOTE();
    switchSetupTimers();

    ledsSetupPWM();

    while(true)
    {
        switch(queueEventDequeue())
        {
        case EventSwitchPressedSingle:
            ledsSetLED2Sat(0);
            break;
        case EventSwitchPressedDouble:
            ledsSetLED2Sat(255);
            break;
        default:
            break;
        }

        NRF_LOG_PROCESS();
        LOG_BACKEND_USB_PROCESS();
    }
}
