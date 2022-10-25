#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "boards.h"

#define DELAY_BLINK 250
#define DELAY_PAUSE 1000

void Nblink(int id, int N)
{
    for(int cntr = 0; cntr < N - 1; ++cntr)
    {
        bsp_board_led_invert(id);
        nrf_delay_ms(DELAY_BLINK);
        bsp_board_led_invert(id);
        nrf_delay_ms(DELAY_BLINK);
    }

    bsp_board_led_invert(id);
    nrf_delay_ms(DELAY_BLINK);
    bsp_board_led_invert(id);
}

int main(void)
{
    bsp_board_init(BSP_INIT_LEDS);

    while (true)
    {
        Nblink(0, 7);
        nrf_delay_ms(DELAY_PAUSE);

        Nblink(1, 1);
        nrf_delay_ms(DELAY_PAUSE);

        Nblink(2, 9);
        nrf_delay_ms(DELAY_PAUSE);

        Nblink(3, 7);
        nrf_delay_ms(DELAY_PAUSE);
    }
}
