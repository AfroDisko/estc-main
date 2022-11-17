#ifndef LEDSUTILS_H
#define LEDSUTILS_H

#include <stdint.h>

typedef struct
{
    uint8_t h;
    uint8_t s;
    uint8_t v;
} ColorHSV;

typedef struct
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} ColorRGB;

ColorRGB hsv2rgb(ColorHSV hsv);

#endif