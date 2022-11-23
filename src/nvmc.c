#include "nrf_bootloader_info.h"
#include "nrf_dfu_types.h"
#include "nrf_log.h"
#include "nrfx_nvmc.h"

#include "nvmc.h"

#define APP_DATA_START_ADDR_P1 (BOOTLOADER_START_ADDR - NRF_DFU_APP_DATA_AREA_SIZE)
#define APP_DATA_START_ADDR_P2 (APP_DATA_START_ADDR_P1 + 4 * 1024)
#define APP_DATA_START_ADDR_P3 (APP_DATA_START_ADDR_P2 + 4 * 1024)

#define WORD_EMPTY 0xffffffff

#define MARK_COLOR 0x00000001
#define MASK_COLOR 0x000000ff

void nvmcWaitWirte(void)
{
    while(!nrfx_nvmc_write_done_check()){}
}

void nvmcSetup(void)
{
    if((*(uint32_t*)APP_DATA_START_ADDR_P1) != 0)
    {
        nrfx_nvmc_page_erase(APP_DATA_START_ADDR_P1);
        nrfx_nvmc_word_write(APP_DATA_START_ADDR_P1, 0);
        nvmcWaitWirte();
    }
}

uint32_t nvmcColor2Word(ColorHSV color)
{
    return (uint32_t)color.h << 24 |
           (uint32_t)color.s << 16 |
           (uint32_t)color.v << 8  |
           MARK_COLOR;
}

void nvmcSaveColor(ColorHSV color)
{
    uint32_t addr = APP_DATA_START_ADDR_P1;
    while(*(uint32_t*)addr != WORD_EMPTY)
    {
        addr += 4;
        if(addr == APP_DATA_START_ADDR_P2)
            return;
    }
    nrfx_nvmc_word_write(addr, nvmcColor2Word(color));
    nvmcWaitWirte();
}

bool nvmcHasColor(void)
{
    uint32_t addr = APP_DATA_START_ADDR_P1;
    while((*(uint32_t*)addr & MASK_COLOR) != MARK_COLOR)
    {
        addr += 4;
        if(addr == APP_DATA_START_ADDR_P2)
            return false;
    }
    return true;
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

ColorHSV nvmcLoadColor(void)
{
    uint32_t addr = APP_DATA_START_ADDR_P1;
    while(*(uint32_t*)addr != WORD_EMPTY)
    {
        addr += 4;
    }
    addr -= 4;
    return nvmcWord2Color(*(uint32_t*)addr);
}
