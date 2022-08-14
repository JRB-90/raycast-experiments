#include "render.h"
#include "SDL.h"
#include "crayconsts.h"
#include "math.h"

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
    
    Vertex offsetPoint = CalculateOffsetPoint(player->frame, PLAYER_ARROW_SIZE);

    res =
        SDL_RenderDrawLineF(
            display->renderer, 
            player->frame.x, 
            player->frame.y,
            player->frame.x + offsetPoint.x,
            player->frame.y + offsetPoint.y
        );

    if (res)
    {
        HandleError("Failed to draw line");
    }

    res =
        SDL_SetRenderDrawColor(
            display->renderer,
            255,
            255,
            255,
            255
        );

    if (res)
    {
        HandleError("Failed to set draw color");
    }

    SDL_Rect rect;
    rect.x = player->frame.x - (PLAYER_BASE_SIZE / 2.0);
    rect.y = player->frame.y - (PLAYER_BASE_SIZE / 2.0);
    rect.w = PLAYER_BASE_SIZE;
    rect.h = PLAYER_BASE_SIZE;

    res =
        SDL_RenderFillRect(
            display->renderer,
            &rect
        );

    if (res)
    {
        HandleError("Failed to draw rectangle");
    }
}
