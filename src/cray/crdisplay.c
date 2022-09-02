#include "crdisplay.h"
#include <assert.h>
#include "crconsts.h"

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

Display CreateDisplay(const char* const title, int width, int height)
{
    Display display =
    {
        .width = width,
        .height = height,
        .window = NULL,
        .renderer = NULL
    };

    assert(SDL_Init(SDL_INIT_EVERYTHING) == 0);

	display.window =
        SDL_CreateWindow(
            title,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            width,
            height,
            SDL_WINDOW_SHOWN
        );

    assert(display.window != NULL);

    display.renderer = SDL_CreateRenderer(display.window, -1, 0);
    assert(display.renderer != NULL);

    return display;
}

void CleanupDisplay(Display display)
{
    SDL_DestroyRenderer(display.renderer);
    SDL_DestroyWindow(display.window);
    SDL_Quit();
}
