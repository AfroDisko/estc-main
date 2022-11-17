#include "nrf_gpio.h"
#include "nrfx_pwm.h"

#include "common.h"
#include "ledsutils.h"
#include "leds.h"

#define LED1_G_PRT 0
#define LED1_G_PIN 6

#define LED2_R_PRT 0
#define LED2_R_PIN 8

#define LED2_G_PRT 1
#define LED2_G_PIN 9

#define LED2_B_PRT 0
#define LED2_B_PIN 12

static volatile uint8_t  gLED1State = 0;
static volatile ColorRGB gLED2State =
{
    .r = 0.,
    .g = 0.,
    .b = 0.
};

static nrf_pwm_values_individual_t gPWMSeqValues = 
{
    .channel_0 = 0,
    .channel_1 = 0,
    .channel_2 = 0,
    .channel_3 = 0
};
static nrf_pwm_sequence_t          gPWMSeq       =
{
    .values.p_individual = &gPWMSeqValues,
    .length              = NRF_PWM_VALUES_LENGTH(gPWMSeqValues),
    .repeats             = 0,
    .end_delay           = 0
};

static const uint16_t    gPWMTopValue = UINT16_MAX;
static nrfx_pwm_t        gPWMInstance = NRFX_PWM_INSTANCE(0);
static nrfx_pwm_config_t gPWMConfig   =
{
    .output_pins =
    {
        NRF_GPIO_PIN_MAP(LED1_G_PRT, LED1_G_PIN) | NRFX_PWM_PIN_INVERTED,
        NRF_GPIO_PIN_MAP(LED2_R_PRT, LED2_R_PIN) | NRFX_PWM_PIN_INVERTED,
        NRF_GPIO_PIN_MAP(LED2_G_PRT, LED2_G_PIN) | NRFX_PWM_PIN_INVERTED,
        NRF_GPIO_PIN_MAP(LED2_B_PRT, LED2_B_PIN) | NRFX_PWM_PIN_INVERTED
    },
    .irq_priority = APP_IRQ_PRIORITY_LOWEST,
    .base_clock   = NRF_PWM_CLK_2MHz,
    .count_mode   = NRF_PWM_MODE_UP,
    .top_value    = gPWMTopValue,
    .load_mode    = NRF_PWM_LOAD_INDIVIDUAL,
    .step_mode    = NRF_PWM_STEP_AUTO
};

uint32_t ledsColor2Pin(char color)
{
    switch(color)
    {
    case 'g':
        return NRF_GPIO_PIN_MAP(LED1_G_PRT, LED1_G_PIN);
    case 'R':
        return NRF_GPIO_PIN_MAP(LED2_R_PRT, LED2_R_PIN);
    case 'G':
        return NRF_GPIO_PIN_MAP(LED2_G_PRT, LED2_G_PIN);
    case 'B':
        return NRF_GPIO_PIN_MAP(LED2_B_PRT, LED2_B_PIN);
    default:
        return 0;
    }
}

void ledsResetPins(void)
{
    nrf_gpio_pin_write(NRF_GPIO_PIN_MAP(LED1_G_PRT, LED1_G_PIN), 1);
    nrf_gpio_pin_write(NRF_GPIO_PIN_MAP(LED2_R_PRT, LED2_R_PIN), 1);
    nrf_gpio_pin_write(NRF_GPIO_PIN_MAP(LED2_G_PRT, LED2_G_PIN), 1);
    nrf_gpio_pin_write(NRF_GPIO_PIN_MAP(LED2_B_PRT, LED2_B_PIN), 1);
}

void ledsSetupGPIO(void)
{
    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(LED1_G_PRT, LED1_G_PIN));
    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(LED2_R_PRT, LED2_R_PIN));
    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(LED2_G_PRT, LED2_G_PIN));
    nrf_gpio_cfg_output(NRF_GPIO_PIN_MAP(LED2_B_PRT, LED2_B_PIN));
    ledsResetPins();
}

void ledsSetLEDState(char color, LogicalState state)
{
    nrf_gpio_pin_write(ledsColor2Pin(color), state == LogicalStateOn ? 0 : 1);
}

LogicalState ledsGetLEDState(char color)
{
    return nrf_gpio_pin_out_read(ledsColor2Pin(color)) == 0 ? LogicalStateOn : LogicalStateOff;
}

void ledsSetupPWM(void)
{
    nrfx_pwm_init(&gPWMInstance, &gPWMConfig, NULL);
    nrfx_pwm_simple_playback(&gPWMInstance, &gPWMSeq, 1, NRFX_PWM_FLAG_LOOP);
}

void ledsUpdatePWMSeqValuesLED1(void)
{
    gPWMSeqValues.channel_0 = gPWMTopValue * gLED1State / UINT8_MAX;
}

void ledsUpdatePWMSeqValuesLED2(void)
{
    gPWMSeqValues.channel_1 = gPWMTopValue * gLED2State.r / UINT8_MAX;
    gPWMSeqValues.channel_2 = gPWMTopValue * gLED2State.g / UINT8_MAX;
    gPWMSeqValues.channel_3 = gPWMTopValue * gLED2State.b / UINT8_MAX;
}

void ledsSetLED2Sat(uint8_t s)
{
    gLED2State = hsv2rgb((ColorHSV){.h = 0, .s = s, .v = 255});
    ledsUpdatePWMSeqValuesLED2();
}
