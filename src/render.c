#include "render.h"
#include <assert.h>
#include "SDL.h"
#include "crayconsts.h"
#include "craymath.h"

void RenderWalls(
    const Display display, 
    const Scene* const scene,
    Color wallColor, 
    Color intersectColor
);
void RenderPlayer(
    const Display display,
    const Player* const player
);

void RenderScene(const Display display, const Scene* const scene)
{
    Color clearColor = CreateColorRGB(0, 0, 0);
    Color wallColor = CreateColorRGB(255, 255, 255);
    Color intersectColor = CreateColorRGB(0, 255, 0);

    ClearScreen(display, clearColor);
    RenderWalls(display, scene, wallColor, intersectColor);
    RenderPlayer(display, &scene->player);
    SDL_RenderPresent(display.renderer);
}

void ClearScreen(const Display display, Color color)
{
    int res =
        SDL_SetRenderDrawColor(
            display.renderer,
            color.r,
            color.g,
            color.b,
            color.a
        );

    assert(res == 0);

    res = SDL_RenderClear(display.renderer);
    assert(res == 0);
}

void RenderWalls(
    const Display display, 
    const Scene* const scene, 
    Color wallColor, 
    Color intersectColor)
{
    if (scene->walls.size < 1)
    {
        return;
    }

    int res =
        SDL_SetRenderDrawColor(
            display.renderer,
            wallColor.r,
            wallColor.g,
            wallColor.b,
            wallColor.a
        );

    assert(res == 0);

    for (int i = 0; i < scene->walls.size; i++)
    {
        LineSegment2D* line = DLLAt(&scene->walls, i);

        res =
            SDL_RenderDrawLineF(
                display.renderer,
                line->p1.x,
                line->p1.y,
                line->p2.x,
                line->p2.y
            );

        assert(res == 0);
    }
}

void RenderPlayer(const Display display, const Player* const player)
{
    int res =
        SDL_SetRenderDrawColor(
            display.renderer,
            player->color.r,
            player->color.g,
            player->color.b,
            player->color.a
        );

    assert(res == 0);
    
    Point2D offsetPoint = 
        CalculateOffsetPoint2D(
            player->frame, 
            PLAYER_ARROW_SIZE
        );

    res =
        SDL_RenderDrawLineF(
            display.renderer, 
            player->frame.position.x,
            player->frame.position.y,
            player->frame.position.x + offsetPoint.x,
            player->frame.position.y + offsetPoint.y
        );

    assert(res == 0);

    res =
        SDL_SetRenderDrawColor(
            display.renderer,
            0,
            0,
            255,
            255
        );

    assert(res == 0);

    SDL_Rect rect;
    rect.x = player->frame.position.x - (PLAYER_BASE_SIZE / 2.0);
    rect.y = player->frame.position.y - (PLAYER_BASE_SIZE / 2.0);
    rect.w = PLAYER_BASE_SIZE;
    rect.h = PLAYER_BASE_SIZE;

    res =
        SDL_RenderFillRect(
            display.renderer,
            &rect
        );

    assert(res == 0);
}
