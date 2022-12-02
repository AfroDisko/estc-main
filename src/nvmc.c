#include "nrf_bootloader_info.h"
#include "nrf_dfu_types.h"
#include "nrfx_nvmc.h"

#include "nvmc.h"

#include "nrf_log.h"

#ifdef  BOOTLOADER_START_ADDR
#undef  BOOTLOADER_START_ADDR
#define BOOTLOADER_START_ADDR 0xE0000
#endif

#define APP_DATA_START_ADDR_P0 (BOOTLOADER_START_ADDR - NRF_DFU_APP_DATA_AREA_SIZE)
#define APP_DATA_START_ADDR_P1 (APP_DATA_START_ADDR_P0 + 1 * CODE_PAGE_SIZE)
#define APP_DATA_START_ADDR_P2 (APP_DATA_START_ADDR_P0 + 2 * CODE_PAGE_SIZE)

#define APP_DATA_PAGES_NUM 3

#define WORD_EMPTY 0xffffffff

#define MASK_COLOR 0x000000ff

#define MARK_COLOR_RGB 0x00000001
#define MARK_COLOR_HSV 0x00000010
#define MARK_COLOR_DEL 0x00000011

static const uint32_t gAppDataStartAddr[] =
{
    APP_DATA_START_ADDR_P0,
    APP_DATA_START_ADDR_P1,
    APP_DATA_START_ADDR_P2
};

static void nvmcAwaitWrite(void)
{
    while(!nrfx_nvmc_write_done_check()){}
}

static void nvmcErasePage(uint8_t pageIdx)
{
    nrfx_nvmc_page_erase(gAppDataStartAddr[pageIdx]);
    nrfx_nvmc_word_write(gAppDataStartAddr[pageIdx], 0);
    nvmcAwaitWrite();
}

void nvmcSetup(bool force)
{
    for(uint8_t pageIdx = 0; pageIdx < APP_DATA_PAGES_NUM; ++pageIdx)
        if((*(uint32_t*)gAppDataStartAddr[pageIdx]) != 0 || force)
            nvmcErasePage(pageIdx);
}

static uint32_t nvmcColorRGB2Word(ColorRGB rgb, char mark)
{
    return (uint32_t)rgb.r << 24 |
           (uint32_t)rgb.g << 16 |
           (uint32_t)rgb.b << 8  |
           mark;
}

static uint32_t nvmcColorHSV2Word(ColorHSV hsv, char mark)
{
    return (uint32_t)hsv.h << 24 |
           (uint32_t)hsv.s << 16 |
           (uint32_t)hsv.v << 8  |
           mark;
}

void nvmcSaveColorHSV(ColorHSV hsv)
{
    uint32_t addr = APP_DATA_START_ADDR_P0;
    while(*(uint32_t*)addr != WORD_EMPTY)
    {
        if(addr == APP_DATA_START_ADDR_P1)
        {
            nvmcErasePage(0);
            nvmcSaveColorHSV(hsv);
        }

        addr += 4;
    }
    nrfx_nvmc_word_write(addr, nvmcColorHSV2Word(hsv, MARK_COLOR_HSV));
    nvmcAwaitWrite();
}

bool nvmcHasColorHSV(void)
{
    uint32_t addr = APP_DATA_START_ADDR_P0;
    while((*(uint32_t*)addr & MASK_COLOR) != MARK_COLOR_HSV)
    {
        if(addr == APP_DATA_START_ADDR_P1)
            return false;

        addr += 4;
    }

    return true;
}

static ColorRGB nvmcWord2ColorRGB(uint32_t word)
{
    ColorRGB rgb =
    {
        .r = (word & 0xff000000) >> 24,
        .g = (word & 0x00ff0000) >> 16,
        .b = (word & 0x0000ff00) >> 8
    };

    return rgb;
}

static ColorHSV nvmcWord2ColorHSV(uint32_t word)
{
    ColorHSV hsv =
    {
        .h = (word & 0xff000000) >> 24,
        .s = (word & 0x00ff0000) >> 16,
        .v = (word & 0x0000ff00) >> 8
    };

    return hsv;
}

ColorHSV nvmcLoadColorHSV(void)
{
    if(!nvmcHasColorHSV())
        return (ColorHSV){0, 0, 0};

    uint32_t addr = APP_DATA_START_ADDR_P0;
    while(*(uint32_t*)(addr + 4) != WORD_EMPTY)
    {
        addr += 4;
    }

    return nvmcWord2ColorHSV(*(uint32_t*)addr);
}

void nvmcSaveColorRGBMarked(ColorRGB rgb, char mark)
{
    uint32_t addr = APP_DATA_START_ADDR_P1;
    uint32_t cntr = UINT32_MAX;
    while(*(uint32_t*)addr != WORD_EMPTY)
    {
        if((*(uint32_t*)addr & MASK_COLOR) != MARK_COLOR_DEL)
            ++cntr;
        if(cntr > 10)
        {
            nvmcErasePage(1);
            nvmcSaveColorRGBMarked(rgb, mark);
        }

        addr += 4;
    }
    NRF_LOG_INFO("%u %c", addr, mark);
    nrfx_nvmc_word_write(addr, nvmcColorRGB2Word(rgb, mark));
    nvmcAwaitWrite();
}

bool nvmcHasColorRGBMarked(char mark)
{
    uint32_t addr = APP_DATA_START_ADDR_P1;
    while((*(uint32_t*)addr & MASK_COLOR) != mark)
    {
        if(addr == APP_DATA_START_ADDR_P2)
            return false;

        addr += 4;
    }

    return true;
}

ColorRGB nvmcLoadColorRGBMarked(char mark)
{
    if(!nvmcHasColorRGBMarked(mark))
        return (ColorRGB){0, 0, 0};

    uint32_t addr = APP_DATA_START_ADDR_P1;
    while((*(uint32_t*)addr & MASK_COLOR) != mark)
    {
        addr += 4;
    }

    return nvmcWord2ColorRGB(*(uint32_t*)addr);
}

void nvmcDeleteColorRGBMarked(char mark)
{
    if(!nvmcHasColorRGBMarked(mark))
        return;

    uint32_t addr = APP_DATA_START_ADDR_P1;
    while((*(uint32_t*)addr & MASK_COLOR) != mark)
    {
        addr += 4;
    }
    nrfx_nvmc_word_write(addr, *(uint32_t*)addr & MARK_COLOR_DEL);
    nvmcAwaitWrite();
}
