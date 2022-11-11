#include <stdbool.h>
#include <string.h>

#include "app_timer.h"

#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_log_backend_usb.h"

#include "switch.h"
#include "leds.h"
#include "queue.h"

int main(void)
{   
    NRF_LOG_INIT(NULL);
    NRF_LOG_DEFAULT_BACKENDS_INIT();

    nrfx_gpiote_init();
    app_timer_init();

    switchSetupSwitch();
    ledsSetupLEDs();

    while(true)
    {
        switch(queueEventDequeue())
        {
        case SW1PressedSingle:
            ledsFlashingStart('Y');
            break;
        case SW1PressedDouble:
            ledsSetLEDState('R', LogicalStateOn);
            ledsSetDutyCycle(0.001);
            break;
        case SW1Released:
            switchExpectPressDouble();
            ledsSetLEDState('R', LogicalStateOff);
            break;
        default:
            break;
        }

        NRF_LOG_PROCESS();
        LOG_BACKEND_USB_PROCESS();
    }
}
