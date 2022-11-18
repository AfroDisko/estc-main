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

#define TIMER_COLOR_MOD_PERIOD_MS 25

APP_TIMER_DEF(gTimerColorMod);

static ColorHSV gColor =
{
    .h = 247,
    .s = 255,
    .v = 255
};

static uint8_t* gColorParam = NULL;

static const char gMode[]  = "NHSV";
static uint8_t    gModeIdx = 0;

void modifyColorParam(void* p_context)
{
    static bool increase = true;

    if(*gColorParam == 255)
        increase = false;

    if(*gColorParam == 0)
        increase = true;

    if(increase)
        ++*gColorParam;
    else
        --*gColorParam;

    ledsSetLED2State(gColor);
}

void switchMode(void)
{
    if(++gModeIdx == strlen(gMode))
        gModeIdx = 0;
    switch(gMode[gModeIdx])
    {
    case 'N':
        gColorParam = NULL;
        ledsFlashLED1Halt();
        ledsSetLED1State(0);
        break;
    case 'H':
        gColorParam = &gColor.h;
        ledsFlashLED1L();
        break;
    case 'S':
        gColorParam = &gColor.s;
        ledsFlashLED1S();
        break;
    case 'V':
        gColorParam = &gColor.v;
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

    app_timer_create(&gTimerColorMod, APP_TIMER_MODE_REPEATED, modifyColorParam);

    switchSetupGPIO();
    switchSetupGPIOTE();
    switchSetupTimers();

    ledsSetupPWM();
    ledsSetupLED1Timer();

    ledsSetLED2State(gColor);

    while(true)
    {
        __WFI();

        switch(queueEventDequeue())
        {
        case EventSwitchPressedContin:
            app_timer_start(gTimerColorMod, APP_TIMER_TICKS(TIMER_COLOR_MOD_PERIOD_MS), NULL);
            break;
        case EventSwitchReleased:
            app_timer_stop(gTimerColorMod);
            break;
        case EventSwitchPressedDouble:
            switchMode();
            break;
        default:
            break;
        }

        NRF_LOG_PROCESS();
        LOG_BACKEND_USB_PROCESS();
    }
}
