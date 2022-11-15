#include <stdbool.h>
#include <string.h>

#include "nrfx_systick.h"
#include "app_timer.h"

#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_log_backend_usb.h"

#include "switch.h"
#include "leds.h"
#include "queue.h"

#define PERIOD_BLINK_MS 1500

char gPatern[] = "YYYYYYYRGGGGGGGGGBBBBBBB";
volatile unsigned int gCurrIdx = 0;

nrfx_timer_t gTimer = NRFX_TIMER_INSTANCE(2);
nrfx_timer_config_t gTimerConfig = NRFX_TIMER_DEFAULT_CONFIG;

void handlerBlink(nrf_timer_event_t event_type, void* p_context)
{
    ledsSmoothBlink(gPatern[gCurrIdx++]);
    if(gCurrIdx == strlen(gPatern))
        gCurrIdx = 0;
}

int main(void)
{   
    NRF_LOG_INIT(NULL);
    NRF_LOG_DEFAULT_BACKENDS_INIT();

    nrfx_systick_init();
    nrfx_gpiote_init();
    app_timer_init();

    nrfx_timer_init(&gTimer, &gTimerConfig, handlerBlink);
    nrfx_timer_extended_compare(&gTimer, NRF_TIMER_CC_CHANNEL0, nrfx_timer_ms_to_ticks(&gTimer, PERIOD_BLINK_MS), NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK, true);

    switchSetupSwitch();
    ledsSetupLEDsGPIO();
    ledsSetupLEDsTimers();

    nrfx_timer_enable(&gTimer);

    while(true)
    {
        switch(queueEventDequeue())
        {
        case SW1PressedSingle:
            break;
        case SW1PressedDouble:
            if(ledsIsSmoothBlinking())
            {
                ledsSmoothBlinkPause();
                nrfx_timer_pause(&gTimer);
            }
            else
            {
                ledsSmoothBlinkResume();
                nrfx_timer_resume(&gTimer);
            }
            break;
        case SW1Released:
            switchExpectPressDouble();
            break;
        default:
            break;
        }

        NRF_LOG_PROCESS();
        LOG_BACKEND_USB_PROCESS();
    }
}
