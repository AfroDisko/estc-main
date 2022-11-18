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

static ColorHSV gDefaultColor =
{
    .h = 247,
    .s = 255,
    .v = 255
};

static const char gMode[]  = "NHSV";
static uint8_t    gModeIdx = 0;

void switchMode(void)
{
    if(++gModeIdx == strlen(gMode))
        gModeIdx = 0;
    switch(gMode[gModeIdx])
    {
    case 'N':
        ledsFlashLED1Halt();
        ledsSetLED1State(0);
        break;
    case 'H':
        ledsFlashLED1L();
        break;
    case 'S':
        ledsFlashLED1S();
        break;
    case 'V':
        ledsFlashLED1Halt();
        ledsSetLED1State(255);
        break;
    default:
        break;
    }
}

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
    ledsSetupLED1Timer();

    ledsSetLED2State(gDefaultColor);

    while(true)
    {
        switch(queueEventDequeue())
        {
        case EventSwitchPressedContin:
            ledsSetLED1State(255);
            break;
        case EventSwitchPressedSingle:
            break;
        case EventSwitchPressedDouble:
            switchMode();
            break;
        case EventSwitchReleased:
            ledsSetLED1State(0);
        default:
            break;
        }

        NRF_LOG_PROCESS();
        LOG_BACKEND_USB_PROCESS();
    }
}
