#include "crdisplay.h"
#include <assert.h>
#include "crconsts.h"
#include "crtime.h"

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
        .window = NULL,
        .renderer = NULL,
        .texture = NULL,
        .screen =
        (ScreenBuffer){
            .pixels = NULL,
            .width = width,
            .height = height
        }
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

    display.renderer = 
        SDL_CreateRenderer(
            display.window, 
            -1, 
            SDL_RENDERER_SOFTWARE
        );

    assert(display.renderer != NULL);

    display.texture =
        SDL_CreateTexture(
            display.renderer,
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_STREAMING,
            width,
            height
        );

    assert(display.texture != NULL);

    size_t pixByteCount = (size_t)width * (size_t)height * 4;
    display.screen.pixels = malloc(pixByteCount);

    assert(display.screen.pixels != NULL);

    return display;
}

void CleanupDisplay(Display* const display)
{
    free(display->screen.pixels);
    SDL_DestroyTexture(display->texture);
    SDL_DestroyRenderer(display->renderer);
    SDL_DestroyWindow(display->window);
    SDL_Quit();
}

void RenderDisplay(const Display* const display, CycleProfile* const profile)
{
    uint64_t presentStartTime = GetTicks();

    int res =
        SDL_UpdateTexture(
            display->texture, 
            NULL, 
            display->screen.pixels,
            display->screen.width * 4
        );

    assert(res == 0);

    res = 
        SDL_RenderCopy(
            display->renderer, 
            display->texture, 
            NULL, 
            NULL
        );

    assert(res == 0);

    SDL_RenderPresent(display->renderer);

    profile->renderPresentTimeMS = GetTimeInMS(GetTicks() - presentStartTime);
}
