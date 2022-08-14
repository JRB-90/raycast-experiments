#include "render.h"
#include "SDL.h"

void RenderPlayer(const Display* display, const Player* player);

void RenderScene(const Display* display, const Scene* scene)
{
    Color clearColor = CreateColor();
    ClearScreen(display, clearColor);
    RenderPlayer(display, &scene->player);
    SDL_RenderPresent(display->renderer);
}

void ClearScreen(const Display* display, Color color)
{
    if (SDL_SetRenderDrawColor(display->renderer, color.r, color.g, color.b, color.a))
    {
        HandleError("Failed to set draw color");
    }

    if (SDL_RenderClear(display->renderer))
    {
        HandleError("Failed to clear renderer");
    }
}

void RenderPlayer(const Display* display, const Player* player)
{
    int res =
        SDL_SetRenderDrawColor(
            display->renderer,
            player->color.r,
            player->color.g,
            player->color.b,
            player->color.a
        );

    if (res)
    {
        HandleError("Failed to set draw color");
    }
    
    res =
        SDL_RenderDrawLineF(
            display->renderer, 
            player->x, 
            player->y,
            player->x,
            player->y + 50
        );

    if (res)
    {
        HandleError("Failed to draw line");
    }
}
