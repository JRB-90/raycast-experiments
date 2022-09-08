#include "crtypes.h"

Color CreateColor()
{
    Color color =
    {
        .a = 255,
        .r = 0,
        .g = 0,
        .b = 0
    };

    return color;
}

Color CreateColorRGB(uint8_t r, uint8_t g, uint8_t b)
{
    Color color =
    {
        .a = 255,
        .r = r,
        .g = g,
        .b = b
    };

    return color;
}

Color CreateColorARGB(uint8_t a, uint8_t r, uint8_t g, uint8_t b)
{
    Color color =
    {
        .a = a,
        .r = r,
        .g = g,
        .b = b
    };

    return color;
}