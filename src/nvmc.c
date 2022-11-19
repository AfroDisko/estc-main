#include "nrf_bootloader_info.h"
#include "nrf_dfu_types.h"
#include "nrf_log.h"
#include "nrfx_nvmc.h"

#include "nvmc.h"

#define APP_DATA_START_ADDR (BOOTLOADER_START_ADDR - NRF_DFU_APP_DATA_AREA_SIZE)

void nvmcWaitWirte(void)
{
    while(!nrfx_nvmc_write_done_check()){}
}

void nvmcPageErase(void)
{
    nrfx_nvmc_page_erase(APP_DATA_START_ADDR);
}

uint32_t nvmcColor2Word(ColorHSV color)
{
    return (uint32_t)color.h << 24 |
           (uint32_t)color.s << 16 |
           (uint32_t)color.v << 8  |
           0x00000000;
}

ColorHSV nvmcWord2Color(uint32_t word)
{
    ColorHSV color =
    {
        .h = (word & 0xff000000) >> 24,
        .s = (word & 0x00ff0000) >> 16,
        .v = (word & 0x0000ff00) >> 8
    };
    return color;
}

void nvmcSaveColor(ColorHSV color)
{
    nrfx_nvmc_page_erase(APP_DATA_START_ADDR);
    nrfx_nvmc_word_write(APP_DATA_START_ADDR, nvmcColor2Word(color));
    nvmcWaitWirte();
}

ColorHSV nvmcLoadColor(void)
{
    ColorHSV color =
    {
        .h = 247,
        .s = 255,
        .v = 255
    };
    if((*(uint32_t*)APP_DATA_START_ADDR & 0x000000ff) != 0x00000000)
        return color;
    else
        return nvmcWord2Color(*(uint32_t*)APP_DATA_START_ADDR);
}
