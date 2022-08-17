#include "render.h"
#include <assert.h>
#include <stdio.h>
#include "SDL.h"
#include "crayconsts.h"
#include "craymath.h"

#define MAX_INTERSECTIONS 10

void RenderWalls(const Display display, const Scene scene);
void RenderPlayer(const Display display, const Scene scene);
void RenderProjection(const Display display, const Scene scene);
void RenderRay(const Display display, const Scene scene, const Vector2D ray);

void RenderScene(const Display display, const Scene scene)
{
    uint64_t start = SDL_GetTicks64();

    ClearScreen(display, scene);
    RenderWalls(display, scene);
    RenderProjection(display, scene);
    RenderPlayer(display, scene);
    SDL_RenderPresent(display.renderer);

    uint64_t timeTaken = SDL_GetTicks64() - start;
    printf("Rendering time: %llums\n", timeTaken);
}

void ClearScreen(const Display display, const Scene scene)
{
    int res =
        SDL_SetRenderDrawColor(
            display.renderer,
            scene.colors.clearCol.r,
            scene.colors.clearCol.g,
            scene.colors.clearCol.b,
            scene.colors.clearCol.a
        );

    assert(res == 0);

    res = SDL_RenderClear(display.renderer);
    assert(res == 0);
}

void RenderWalls(const Display display, const Scene scene)
{
    if (scene.walls.size < 1)
    {
        return;
    }

    int res =
        SDL_SetRenderDrawColor(
            display.renderer,
            scene.colors.wallCol.r,
            scene.colors.wallCol.g,
            scene.colors.wallCol.b,
            scene.colors.wallCol.a
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
            scene.colors.playerCol.r,
            scene.colors.playerCol.g,
            scene.colors.playerCol.b,
            scene.colors.playerCol.a
        );

    assert(res == 0);
    
    SDL_Rect rect =
    {
        .x = scene.player.frame.position.x - (PLAYER_BASE_SIZE / 2.0),
        .y = scene.player.frame.position.y - (PLAYER_BASE_SIZE / 2.0),
        .w = PLAYER_BASE_SIZE,
        .h = PLAYER_BASE_SIZE
    };

    res =
        SDL_RenderFillRect(
            display.renderer,
            &rect
        );

    assert(res == 0);
}

void RenderProjection(const Display display, const Scene scene)
{
    Vector2D worldForward = { .x = 0.0, .y = -1.0 };
    int widthIncrements = 9;
    double angleInterval = (scene.player.fov * 2.0) / ((double)(widthIncrements - 1));
    double startAngle = scene.player.frame.theta - scene.player.fov;

    for (int i = 0; i < widthIncrements; i++)
    {
        double theta = startAngle + (i * angleInterval);
        
        Vector2D lookDir =
            FindLookVector(
                worldForward,
                theta
            );

        RenderRay(
            display,
            scene,
            Vec2DNormalise(lookDir)
        );
    }
}

void RenderRay(const Display display, const Scene scene, const Vector2D ray)
{
    int pointIndex = 0;
    double intersectionDistances[MAX_INTERSECTIONS];
    Point2D intersectedPoints[MAX_INTERSECTIONS];
    DLLNode* current = scene.walls.head;

    while (current != NULL)
    {
        const LineSegment2D* const line = (LineSegment2D*)current->data;
        Point2D intersectionPoint;
        double distanceToLine;

        bool doesIntersect =
            DoesRayInterectLine(
                scene.player.frame.position,
                ray,
                *line,
                &distanceToLine,
                &intersectionPoint
            );

        if (doesIntersect)
        {
            intersectionDistances[pointIndex] = distanceToLine;
            intersectedPoints[pointIndex] = intersectionPoint;
            pointIndex++;

            if (pointIndex >= MAX_INTERSECTIONS)
            {
                break;
            }
        }

        current = current->next;
    }

    if (pointIndex == 0)
    {
        return;
    }

    int shortestIndex = 0;
    for (int i = 0; i < pointIndex; i++)
    {
        if (intersectionDistances[i] <
            intersectionDistances[shortestIndex])
        {
            shortestIndex = i;
        }
    }

    int res =
        SDL_SetRenderDrawColor(
            display.renderer,
            scene.colors.rayCol.r,
            scene.colors.rayCol.g,
            scene.colors.rayCol.b,
            scene.colors.rayCol.a
        );

    assert(res == 0);

    res =
        SDL_RenderDrawLineF(
            display.renderer,
            scene.player.frame.position.x,
            scene.player.frame.position.y,
            intersectedPoints[shortestIndex].x,
            intersectedPoints[shortestIndex].y
        );

    assert(res == 0);

    res =
        SDL_SetRenderDrawColor(
            display.renderer,
            scene.colors.intersectCol.r,
            scene.colors.intersectCol.g,
            scene.colors.intersectCol.b,
            scene.colors.intersectCol.a
        );

    assert(res == 0);

    SDL_Rect rect;
    rect.x = intersectedPoints[shortestIndex].x - 2;
    rect.y = intersectedPoints[shortestIndex].y - 2;
    rect.w = 4;
    rect.h = 4;

    res =
        SDL_RenderFillRect(
            display.renderer,
            &rect
        );

    assert(res == 0);
}
