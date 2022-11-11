#include <stdbool.h>
#include <string.h>

#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_log_backend_usb.h"

#include "switch.h"
#include "leds.h"
#include "queue.h"

int main(void)
{   
    app_timer_init();
    nrfx_gpiote_init();

    switchSetupSwitch();
    ledsSetupLEDs();

    APP_ERROR_CHECK(NRF_LOG_INIT(NULL));
    NRF_LOG_DEFAULT_BACKENDS_INIT();

    while(true)
    {
        switch(queueEventDequeue())
        {
        case DummyEvent:
            break;
        case SW1PressedSingle:
            ledsSetLEDState('Y', LogicalStateOn);
            break;
        case SW1PressedDouble:
            ledsSetLEDState('R', LogicalStateOn);
            break;
        case SW1Released:
            switchExpectPressDouble();
            ledsSetLEDState('Y', LogicalStateOff);
            ledsSetLEDState('R', LogicalStateOff);
            break;
        }

        NRF_LOG_PROCESS();
        LOG_BACKEND_USB_PROCESS();
    }
}
