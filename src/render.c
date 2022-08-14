#include "render.h"
#include "SDL.h"

void RenderPlayer(const Display* display, const Player* player);

void RenderScene(const Display* display, const Scene* scene)
{
    Color clearColor = CreateColor();
    ClearScreen(display, clearColor);
    RenderPlayer(display, &scene->player);
    SDL_UpdateWindowSurface(display->window);
}

void ClearScreen(const Display* display, Color color)
{
    if (SDL_MUSTLOCK(display->surface))
    {
        SDL_LockSurface(display->surface);
    }

    int res =
        SDL_FillRect(
            display->surface,
            NULL,
            SDL_MapRGB(
                display->surface->format,
                color.r,
                color.g,
                color.b
            )
        );

    if (res)
    {
        HandleError("Failed to clear screen");
    }
}

void RenderPlayer(const Display* display, const Player* player)
{
    // TODO
}
