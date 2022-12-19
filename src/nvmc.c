#include "nrf_bootloader_info.h"
#include "nrf_dfu_types.h"
#include "nrfx_nvmc.h"

#include "nrf_log.h"

#include "metadata.h"
#include "nvmc.h"

#ifdef  BOOTLOADER_START_ADDR
#undef  BOOTLOADER_START_ADDR
#define BOOTLOADER_START_ADDR 0xE0000
#endif

#define APP_DATA_START_ADDR_P0 (BOOTLOADER_START_ADDR - NRF_DFU_APP_DATA_AREA_SIZE)
#define APP_DATA_START_ADDR_P1 (APP_DATA_START_ADDR_P0 + 1 * CODE_PAGE_SIZE)
#define APP_DATA_START_ADDR_P2 (APP_DATA_START_ADDR_P0 + 2 * CODE_PAGE_SIZE)

#define APP_DATA_PAGES_NUM 3

#define OFFSET_DATA 2

typedef enum
{
    nvmcRetCodeSuccess,
    nvmcRetCodeBeyondPage,
    nvmcRetCodeMetaNotFound
} nvmcRetCode;

static const uint32_t gAppDataStartAddr[] =
{
    APP_DATA_START_ADDR_P0,
    APP_DATA_START_ADDR_P1,
    APP_DATA_START_ADDR_P2
};

static const Metadata gMetadataNone =
{
    .type   = METADATA_TYPE_NONE,
    .state  = METADATA_STATE_NONE,
    .length = UINT8_MAX
};

static const Metadata gMetadataPage =
{
    .type   = METADATA_TYPE_PAGE_INFO,
    .state  = METADATA_STATE_ACTIVE,
    .length = 0
};

static void nvmcAwaitWrite(void)
{
    while(!nrfx_nvmc_write_done_check()){}
}

static void nvmcMetadataWrite(uint32_t addr, Metadata meta)
{
    uint8_t bytes[2];
    bytes[0] = (meta.type & METADATA_MASK_TYPE) | (meta.state & METADATA_MASK_STATE);
    bytes[1] = meta.length;

    nrfx_nvmc_bytes_write(addr, bytes, 2);
    nvmcAwaitWrite();
}

static Metadata nvmcMetadataRead(uint32_t addr)
{
    Metadata meta =
    {
        .type   = *(uint8_t*)addr & METADATA_MASK_TYPE,
        .state  = *(uint8_t*)addr & METADATA_MASK_STATE,
        .length = *(uint8_t*)(addr + 1)
    };
    return meta;
}

static uint32_t nvmcGetNextAddr(uint8_t pageIdx, uint32_t addrCurr)
{
    Metadata meta = nvmcMetadataRead(addrCurr);
    return addrCurr + OFFSET_DATA + meta.length;
}

static void nvmcPageErase(uint8_t pageIdx)
{
    nrfx_nvmc_page_erase(gAppDataStartAddr[pageIdx]);
    nvmcMetadataWrite(gAppDataStartAddr[pageIdx], gMetadataPage);
}

void nvmcSetup(bool force)
{
    for(uint8_t pageIdx = 0; pageIdx < APP_DATA_PAGES_NUM; ++pageIdx)
    {
        Metadata meta = nvmcMetadataRead(gAppDataStartAddr[pageIdx]);
        if(!metadataIsEqual(&meta, &gMetadataPage) || force)
            nvmcPageErase(pageIdx);
    }
}

static nvmcRetCode nvmcRecordWrite(uint8_t pageIdx, uint32_t addr, Metadata meta, const uint8_t* data)
{
    if(OFFSET_DATA + meta.length > gAppDataStartAddr[pageIdx] + CODE_PAGE_SIZE - addr)
        return nvmcRetCodeBeyondPage;

    nvmcMetadataWrite(addr, meta);
    nrfx_nvmc_bytes_write(addr + OFFSET_DATA, data, meta.length);
    nvmcAwaitWrite();

    return nvmcRetCodeSuccess;
}

static nvmcRetCode nvmcRecordFindLast(uint8_t pageIdx, uint32_t* addr)
{
    uint32_t addrCurr = gAppDataStartAddr[pageIdx];
    uint32_t addrNext = nvmcGetNextAddr(pageIdx, addrCurr);

    Metadata metaNext = nvmcMetadataRead(addrNext);

    while(!metadataIsEqual(&metaNext, &gMetadataNone))
    {
        addrCurr = addrNext;
        addrNext = nvmcGetNextAddr(pageIdx, addrCurr);
        if(addrNext >= gAppDataStartAddr[pageIdx] + CODE_PAGE_SIZE)
        {
            *addr = addrCurr;
            return nvmcRetCodeSuccess;
        }

        metaNext = nvmcMetadataRead(addrNext);
    }

    *addr = addrCurr;
    return nvmcRetCodeSuccess;
}

static nvmcRetCode nvmcRecordFindLastMeta(uint8_t pageIdx, uint32_t* addr, Metadata metaRef)
{
    uint32_t addrCurr = gAppDataStartAddr[pageIdx];
    uint32_t addrNext = nvmcGetNextAddr(pageIdx, addrCurr);

    Metadata metaCurr = nvmcMetadataRead(addrCurr);
    Metadata metaNext = nvmcMetadataRead(addrNext);

    *addr = gAppDataStartAddr[pageIdx];

    while(!metadataIsEqual(&metaNext, &gMetadataNone))
    {
        addrCurr = addrNext;
        addrNext = nvmcGetNextAddr(pageIdx, addrCurr);
        if(addrNext >= gAppDataStartAddr[pageIdx] + CODE_PAGE_SIZE)
            return nvmcRetCodeSuccess;

        metaCurr = nvmcMetadataRead(addrCurr);
        metaNext = nvmcMetadataRead(addrNext);

        if(metadataIsEqual(&metaCurr, &metaRef))
            *addr = addrCurr;
    }

    if(*addr > gAppDataStartAddr[pageIdx])
        return nvmcRetCodeSuccess;
    else
        return nvmcRetCodeMetaNotFound;
}

static nvmcRetCode nvmcRecordFindFree(uint8_t pageIdx, uint32_t* addr)
{
    uint32_t addrCurr = gAppDataStartAddr[pageIdx];
    uint32_t addrNext = nvmcGetNextAddr(pageIdx, addrCurr);

    Metadata metaNext = nvmcMetadataRead(addrNext);

    while(!metadataIsEqual(&metaNext, &gMetadataNone))
    {
        addrCurr = addrNext;
        addrNext = nvmcGetNextAddr(pageIdx, addrCurr);

        if(addrNext >= gAppDataStartAddr[pageIdx] + CODE_PAGE_SIZE)
            return nvmcRetCodeBeyondPage;

        metaNext = nvmcMetadataRead(addrNext);
    }

    *addr = addrNext;
    return nvmcRetCodeSuccess;
}

void nvmcSaveColorHSV(ColorHSV hsv)
{
    uint32_t addr;
    if(nvmcRecordFindFree(0, &addr) == nvmcRetCodeBeyondPage)
    {
        nvmcPageErase(0);
        nvmcRecordFindFree(0, &addr);
    }

    Metadata meta =
    {
        .type   = METADATA_TYPE_COLOR_HSV,
        .state  = METADATA_STATE_ACTIVE,
        .length = 3
    };

    uint8_t data[3];
    data[0] = hsv.h;
    data[1] = hsv.s;
    data[2] = hsv.v;

    nvmcRecordWrite(0, addr, meta, data);
}

void nvmcLoadColorHSV(ColorHSV* hsv)
{
    Metadata meta =
    {
        .type   = METADATA_TYPE_COLOR_HSV,
        .state  = METADATA_STATE_ACTIVE,
        .length = 3
    };

    uint32_t addr;
    if(nvmcRecordFindLastMeta(0, &addr, meta) == nvmcRetCodeSuccess)
    {
        hsv->h = *(uint8_t*)(addr + OFFSET_DATA);
        hsv->s = *(uint8_t*)(addr + OFFSET_DATA + 1);
        hsv->v = *(uint8_t*)(addr + OFFSET_DATA + 2);
    }
}

// void nvmcSaveColorRGBNamed(ColorRGB rgb, char* name)
// {

// }

// bool nvmcHasColorRGBMarked(char mark)
// {
//     uint32_t addr = APP_DATA_START_ADDR_P1;
//     while((*(uint32_t*)addr & MASK_COLOR) != mark)
//     {
//         if(addr == APP_DATA_START_ADDR_P2)
//             return false;

//         addr += 4;
//     }

//     return true;
// }

// ColorRGB nvmcLoadColorRGBMarked(char mark)
// {
//     if(!nvmcHasColorRGBMarked(mark))
//         return (ColorRGB){0, 0, 0};

//     uint32_t addr = APP_DATA_START_ADDR_P1;
//     while((*(uint32_t*)addr & MASK_COLOR) != mark)
//     {
//         addr += 4;
//     }

//     return nvmcWord2ColorRGB(*(uint32_t*)addr);
// }

// void nvmcDeleteColorRGBMarked(char mark)
// {
//     if(!nvmcHasColorRGBMarked(mark))
//         return;

//     uint32_t addr = APP_DATA_START_ADDR_P1;
//     while((*(uint32_t*)addr & MASK_COLOR) != mark)
//     {
//         addr += 4;
//     }
//     nrfx_nvmc_word_write(addr, *(uint32_t*)addr & MARK_COLOR_DEL);
//     nvmcAwaitWrite();
// }
