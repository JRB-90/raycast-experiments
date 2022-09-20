#include "crtypes.h"
#include <stddef.h>

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

uint16_t ToUint16Color(const Color* const color)
{
    uint16_t r2 = (color->r / 2) << 12;
    uint16_t g2 = (color->g / 2) << 8;
    uint16_t b2 = (color->b / 2) << 4;
    uint16_t a2 = (color->a / 2);

    return a2 | b2 | g2 | r2;
}

uint32_t ToUint32Color(const Color* const color)
{
    uint32_t r2 = color->r << 24;
    uint32_t g2 = color->g << 16;
    uint32_t b2 = color->b << 8;
    uint32_t a2 = color->a;

    return r2 | g2 | b2 | a2;
}

InputState DefaultInputState()
{
    InputState inputState =
    {
        .quit = false,
        .forwards = false,
        .backwards = false,
        .rotLeft = false,
        .rotRight = false,
        .left = false,
        .right = false,
        .toggleDebug = false,
        .toggleRenderMode = false,
    };

    return inputState;
}

ScreenBuffer DefaultScreen()
{
    ScreenBuffer screen =
    {
        .pixels = NULL,
        .size = -1,
        .width = -1,
        .height = -1,
        .stride = -1,
        .bitsPP = -1,
        .bytesPP = -1
    };

    return screen;
}
