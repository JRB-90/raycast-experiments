#include "crsdl2_display.h"
#include <inttypes.h>
#include <string.h>
#include <assert.h>
#include "crtime.h"
#include "SDL.h"

typedef struct SdlDisplay {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
} SdlDisplay;

SdlDisplay display;

int CreateDisplay(
    const char* const title,
    const ScreenFormat* const desiredFormat,
    ScreenBuffer* const screen
);
int CleanupDisplay(ScreenBuffer* const screen);
int RenderDisplayInternal(ScreenBuffer* const screen);
Uint32 ToSdlFormatFlag(const ColorFormat colorFormat);

int InitDisplay(
    const ScreenFormat* const desiredFormat, 
    ScreenBuffer* const screen)
{
    return
        CreateDisplay(
            "SDL2 C Raycast Demo",
            desiredFormat,
            screen
        );
}

int DestroyDisplay(ScreenBuffer* const screen)
{
    return CleanupDisplay(screen);
}

int RenderDisplay(ScreenBuffer* const screen)
{
    return RenderDisplayInternal(screen);
}

int CreateDisplay(
    const char* const title,
    const ScreenFormat* const desiredFormat,
    ScreenBuffer* const screen)
{
    display =
    (SdlDisplay){
        .window = NULL,
        .renderer = NULL,
        .texture = NULL
    };

    if (SDL_Init(SDL_INIT_EVERYTHING))
    {
        return -1;
    }

	display.window =
        SDL_CreateWindow(
            title,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            desiredFormat->width,
            desiredFormat->height,
            SDL_WINDOW_SHOWN
        );

    if (display.window == NULL)
    {
        return -1;
    }

    display.renderer = 
        SDL_CreateRenderer(
            display.window, 
            -1, 
            SDL_RENDERER_SOFTWARE
        );

    if (display.renderer == NULL)
    {
        return -1;
    }

    display.texture =
        SDL_CreateTexture(
            display.renderer,
            ToSdlFormatFlag(desiredFormat->format),
            SDL_TEXTUREACCESS_STREAMING,
            desiredFormat->width,
            desiredFormat->height
        );

    if (display.texture == NULL)
    {
        return -1;
    }

    size_t pixByteCount = 
        (size_t)desiredFormat->width * 
        (size_t)desiredFormat->height * 
        4;

    screen->pixels = malloc(pixByteCount);
    if (screen->pixels == NULL)
    {
        return -1;
    }

    screen->width = desiredFormat->width;
    screen->height = desiredFormat->height;
    screen->bitsPP = ToBitsPerPixel(desiredFormat->format);
    screen->bytesPP = screen->bitsPP / 8;
    screen->stride = screen->width * screen->bytesPP;
    screen->size = screen->stride * screen->height;
    screen->offset = 0;
    screen->colorFormat = desiredFormat->format;

    return 0;
}

int CleanupDisplay(ScreenBuffer* const screen)
{
    free(screen->pixels);
    SDL_DestroyTexture(display.texture);
    SDL_DestroyRenderer(display.renderer);
    SDL_DestroyWindow(display.window);
    SDL_Quit();

    return 0;
}

int RenderDisplayInternal(ScreenBuffer* const screen)
{
    int res = SDL_RenderClear(display.renderer);
    if (res)
    {
        return -1;
    }

    uint8_t* texturePixels = NULL;
    int pitch = -1;

    res =
        SDL_LockTexture(
            display.texture,
            NULL,
            (void**)&texturePixels,
            &pitch
        );

    if (res)
    {
        return -1;
    }

    memcpy(texturePixels, screen->pixels, pitch * screen->height);

    SDL_UnlockTexture(display.texture);

    res =
        SDL_RenderCopy(
            display.renderer, 
            display.texture, 
            NULL, 
            NULL
        );

    if (res)
    {
        return -1;
    }

    SDL_RenderPresent(display.renderer);

    return 0;
}

Uint32 ToSdlFormatFlag(const ColorFormat colorFormat)
{
    switch (colorFormat)
    {
    case CF_ARGB:
        return SDL_PIXELFORMAT_ARGB8888;
    case CF_RGBA:
        return SDL_PIXELFORMAT_RGBA8888;
    case CF_RGB565:
        return SDL_PIXELFORMAT_RGB565;
    default:
        return SDL_PIXELFORMAT_RGBA8888;
    }
}
