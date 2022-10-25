#include <stdbool.h>
#include <stdint.h>
#include "nrf_delay.h"
#include "boards.h"

#define DELAY_BLINK 250
#define DELAY_PAUSE 1000

void nBlink(int idx, int n)
{
    for(int cntr = 0; cntr < 2 * n - 1; ++cntr)
    {
        bsp_board_led_invert(idx);
        nrf_delay_ms(DELAY_BLINK);
    }

    // just in case
    if(n > 0) 
        bsp_board_led_invert(idx);
}

int main(void)
{
    bsp_board_init(BSP_INIT_LEDS);

    int nBlinks[4] = {7, 1, 9, 7};

    while(true)
    {
        for(int idx = 0; idx < LEDS_NUMBER; ++idx)
        {
            nBlink(idx, nBlinks[idx]);
            nrf_delay_ms(DELAY_PAUSE);
        }
    }
}
