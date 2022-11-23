#ifndef NVMC_H
#define NVMC_H

#include "ledsutils.h"

void nvmcSetup(void);

void nvmcSaveColor(ColorHSV color);

bool nvmcHasColor(void);

ColorHSV nvmcLoadColor(void);

#endif