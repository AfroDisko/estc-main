#include "app_timer.h"

#include "switch.h"
#include "queue.h"

#define SW1_PRT 1
#define SW1_PIN 6

#define SWITCH_DEBOUNCE_TIME_MS 50

APP_TIMER_DEF(switchTimerDebounce);

APP_TIMER_DEF(switchTimerPressDouble);

LogicalState switchGetSwitchState(void)
{
    return nrf_gpio_pin_read(NRF_GPIO_PIN_MAP(SW1_PRT, SW1_PIN)) == 0 ? LogicalStateOn : LogicalStateOff;
}

void switchSetupSwitchEvent(nrfx_gpiote_in_config_t* config, nrfx_gpiote_evt_handler_t handler)
{
    config->pull = NRF_GPIO_PIN_PULLUP;
    nrfx_gpiote_in_init(NRF_GPIO_PIN_MAP(SW1_PRT, SW1_PIN), config, handler);
}

void switchEnableEvent(bool int_enable)
{
    nrfx_gpiote_in_event_enable(NRF_GPIO_PIN_MAP(SW1_PRT, SW1_PIN), int_enable);
}

void switchSetupSwitch(void)
{
    nrf_gpio_cfg_input(NRF_GPIO_PIN_MAP(SW1_PRT, SW1_PIN), NRF_GPIO_PIN_PULLUP);

    nrfx_gpiote_in_config_t config = NRFX_GPIOTE_CONFIG_IN_SENSE_TOGGLE(true);
    switchSetupSwitchEvent(&config, switchHandlerSW1);
    switchEnableEvent(true);

    app_timer_create(&switchTimerDebounce, APP_TIMER_MODE_SINGLE_SHOT, switchHandlerDebounce);
    app_timer_create(&switchTimerPressDouble, APP_TIMER_MODE_SINGLE_SHOT, switchHandlerPressDouble);
}

void switchExpectPressDouble(void)
{
    app_timer_start(switchTimerPressDouble, APP_TIMER_TICKS(SWITCH_DEBOUNCE_TIME_MS), NULL);
}

void switchHandlerSW1(nrfx_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
{
    app_timer_start(switchTimerDebounce, APP_TIMER_TICKS(SWITCH_DEBOUNCE_TIME_MS), NULL);       
}

void switchHandlerDebounce()
{
    queueEventEnqueue(switchGetSwitchState() == LogicalStateOn ? SW1PressedSingle : SW1Released);
}

void switchHandlerPressDouble()
{
    if(switchGetSwitchState() == LogicalStateOn)
        queueEventEnqueue(SW1PressedDouble);
}
