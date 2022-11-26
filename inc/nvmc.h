#ifndef NVMC_H
#define NVMC_H

#include <stdbool.h>

#include "ledsutils.h"

void nvmcSetup(bool force);

void nvmcSaveColor(ColorHSV color);

bool nvmcHasColor(void);

ColorHSV nvmcLoadColor(void);

#endif
