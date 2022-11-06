#ifndef SWITCH2GPIOTE_H
#define SWITCH2GPIOTE_H

#include "nrfx_gpiote.h"

void switch2gpioteSetupSwitch(nrfx_gpiote_in_config_t const* config, nrfx_gpiote_evt_handler_t handler);

void switch2gpioteEventEnable(bool int_enable);

#endif
