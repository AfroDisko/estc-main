#ifndef NVMC_H
#define NVMC_H

#include <stdbool.h>

#include "ledsutils.h"
#include "metadata.h"

typedef enum
{
    nvmcRetCodeSuccess,
    nvmcRetCodeBeyondPage,
    nvmcRetCodeMetaNotFound
} nvmcRetCode;

void nvmcSetup(bool force);

void nvmcSaveColorHSV(ColorHSV hsv);

void nvmcLoadColorHSV(ColorHSV* hsv);

void nvmcSaveColorRGBNamed(ColorRGB rgb, const char* name);

nvmcRetCode nvmcLoadColorRGBNamed(ColorRGB* rgb, const char* mark);

nvmcRetCode nvmcDeleteColorRGBNamed(const char* name);

uint32_t nvmcRecordCountMeta(uint8_t pageIdx, Metadata metaRef);

#endif
