#ifndef NVMC_H
#define NVMC_H

#include <stdbool.h>

#include "ledsutils.h"

void nvmcSetup(bool force);

void nvmcSaveColorHSV(ColorHSV hsv);

void nvmcLoadColorHSV(ColorHSV* hsv);

void nvmcSaveColorRGBMarked(ColorRGB rgb, char mark);

bool nvmcHasColorRGBMarked(char mark);

ColorRGB nvmcLoadColorRGBMarked(char mark);

void nvmcDeleteColorRGBMarked(char mark);

#endif
