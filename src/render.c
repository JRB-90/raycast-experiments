#include "render.h"
#include <assert.h>
#include "SDL.h"
#include "crayconsts.h"
#include "craymath.h"

void RenderWalls(
    const Display display, 
    const Scene scene,
    const Color wallColor,
    const Color intersectColor
);
void RenderPlayer(
    const Display display,
    const Scene scene
);

void RenderScene(const Display display, const Scene scene)
{
    Color clearColor = CreateColorRGB(0, 0, 0);
    Color wallColor = CreateColorRGB(255, 255, 255);
    Color intersectColor = CreateColorRGB(0, 255, 0);

    ClearScreen(display, clearColor);
    RenderWalls(display, scene, wallColor, intersectColor);
    RenderPlayer(display, scene);
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
    const Scene scene, 
    Color wallColor, 
    Color intersectColor)
{
    if (scene.walls.size < 1)
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

    for (int i = 0; i < scene.walls.size; i++)
    {
        LineSegment2D* line = DLLAt(&scene.walls, i);

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

void RenderPlayer(const Display display, const Scene scene)
{
    int res =
        SDL_SetRenderDrawColor(
            display.renderer,
            scene.player.color.r,
            scene.player.color.g,
            scene.player.color.b,
            scene.player.color.a
        );

    assert(res == 0);
    
    Vector2D worldForward = { .x = 0.0, .y = -1.0 };
    Vector2D lookDir = 
        FindLookVector(
            worldForward, 
            scene.player.frame.theta
        );

    lookDir = Vec2DNormalise(lookDir);

    Point2D offsetPoint =
        AddVec2DToPoint2D(
            scene.player.frame.position,
            Vec2DMul(lookDir, PLAYER_ARROW_SIZE)
        );

    res =
        SDL_RenderDrawLineF(
            display.renderer,
            scene.player.frame.position.x,
            scene.player.frame.position.y,
            offsetPoint.x,
            offsetPoint.y
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
    rect.x = scene.player.frame.position.x - (PLAYER_BASE_SIZE / 2.0);
    rect.y = scene.player.frame.position.y - (PLAYER_BASE_SIZE / 2.0);
    rect.w = PLAYER_BASE_SIZE;
    rect.h = PLAYER_BASE_SIZE;

    res =
        SDL_RenderFillRect(
            display.renderer,
            &rect
        );

    assert(res == 0);

    DLLNode* current = scene.walls.head;

    while (current != NULL)
    {
        const LineSegment2D* const line = (LineSegment2D*)current->data;
        Point2D intersectionPoint;

        bool doesIntersect =
            DoesRayInterectLine(
                scene.player.frame.position,
                lookDir,
                *line,
                &intersectionPoint
            );

        if (doesIntersect)
        {
            res =
                SDL_SetRenderDrawColor(
                    display.renderer,
                    0,
                    255,
                    0,
                    255
                );

            assert(res == 0);

            SDL_Rect rect;
            rect.x = intersectionPoint.x - 2;
            rect.y = intersectionPoint.y - 2;
            rect.w = 4;
            rect.h = 4;

            res =
                SDL_RenderFillRect(
                    display.renderer,
                    &rect
                );

            assert(res == 0);
        }

        current = current->next;
    }
}
