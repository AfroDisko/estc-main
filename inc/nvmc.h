#ifndef NVMC_H
#define NVMC_H

#include "ledsutils.h"

void nvmcPageErase(void);

void nvmcSaveColor(ColorHSV color);

ColorHSV nvmcLoadColor(void);

#endif