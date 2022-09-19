#include "crsdl2_display.h"
#include <string.h>
#include <assert.h>
#include "crconsts.h"
#include "crtime.h"

SdlDisplay display;

int CreateDisplay(
    ScreenBuffer* const screen, 
    const char* const title, 
    int width, 
    int height
);
int CleanupDisplay(ScreenBuffer* const screen);
int RenderDisplayInternal(
    ScreenBuffer* const screen, 
    CycleProfile* const profile
);

int InitDisplay(ScreenBuffer* const screen)
{
    return
        CreateDisplay(
            screen,
            "SDL2 C Raycast Demo",
            640,
            480
        );
}

int DestroyDisplay(ScreenBuffer* const screen)
{
    return CleanupDisplay(screen);
}

int RenderDisplay(
    ScreenBuffer* const screen,
    CycleProfile* const profile)
{
    return RenderDisplayInternal(screen, profile);
}

int CreateDisplay(
    ScreenBuffer* const screen, 
    const char* const title, 
    int width, 
    int height)
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
            width,
            height,
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
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_STREAMING,
            width,
            height
        );

    if (display.texture == NULL)
    {
        return -1;
    }

    size_t pixByteCount = (size_t)width * (size_t)height * 4;
    screen->pixels = malloc(pixByteCount);
    if (screen->pixels == NULL)
    {
        return -1;
    }

    screen->width = width;
    screen->height = height;
    screen->bitsPP = 32;
    screen->bytesPP = 32;
    screen->stride = screen->width * screen->bytesPP;
    screen->size = screen->stride * screen->height;

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

int RenderDisplayInternal(
    ScreenBuffer* const screen,
    CycleProfile* const profile)
{
    uint64_t presentStartTime = GetTicks();

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

    profile->renderPresentTimeMS = GetTimeInMS(GetTicks() - presentStartTime);

    return 0;
}
