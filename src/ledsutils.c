#include "ledsutils.h"

ColorRGB hsv2rgb(ColorHSV hsv)
{
    ColorRGB rgb;

    if(hsv.s == 0)
    {
        rgb.r = hsv.v;
        rgb.g = hsv.v;
        rgb.b = hsv.v;
        return rgb;
    }

    uint8_t reg = hsv.h / 43;
    uint8_t res = (hsv.h - (reg * 43)) * 6; 

    uint8_t p = (hsv.v * (UINT8_MAX - hsv.s)) >> 8;
    uint8_t q = (hsv.v * (UINT8_MAX - ((hsv.s * res) >> 8))) >> 8;
    uint8_t t = (hsv.v * (UINT8_MAX - ((hsv.s * (UINT8_MAX - res)) >> 8))) >> 8;

    switch(reg)
    {
    case 0:
        rgb.r = hsv.v;
        rgb.g = t;
        rgb.b = p;
        break;
    case 1:
        rgb.r = q;
        rgb.g = hsv.v;
        rgb.b = p;
        break;
    case 2:
        rgb.r = p;
        rgb.g = hsv.v;
        rgb.b = t;
        break;
    case 3:
        rgb.r = p;
        rgb.g = q;
        rgb.b = hsv.v;
        break;
    case 4:
        rgb.r = t;
        rgb.g = p;
        rgb.b = hsv.v;
        break;
    default:
        rgb.r = hsv.v;
        rgb.g = p;
        rgb.b = q;
        break;
    }

    return rgb;
}
