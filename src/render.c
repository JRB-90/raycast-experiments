#include "render.h"
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include "SDL.h"
#include "crayconsts.h"
#include "craymath.h"

#define MAX_INTERSECTIONS 10

void RenderWalls(const Display display, const Scene scene);
void RenderPlayer(const Display display, const Scene scene);
void RenderProjection(const Display display, const Scene scene);
void RenderRay(const Display display, const Scene scene, const Vector2D ray);

void RenderCameraSpaceLine(
    const Frame2D* const cameraFrame,
    const Display* const display,
    const Color* const color,
    const double x1,
    const double y1,
    const double x2,
    const double y2
);
void RenderScreenSpaceLine(
    const Display* const display,
    const Color* const color,
    const double x1,
    const double y1,
    const double x2,
    const double y2
);
void RenderCameraSpaceRectangle(
    const Frame2D* const cameraFrame,
    const Display* const display,
    const Color* const color,
    const SDL_Rect* const area,
    bool fill
);
void RenderScreenSpaceRectangle(
    const Display* const display,
    const Color* const color,
    const SDL_Rect* const area,
    bool fill
);

void RenderTile(const Display display, const Scene scene, const DisplayTile tile)
{
    ClearScreen(display, scene);
    SDL_RenderSetViewport(display.renderer, &tile.position);
    RenderScene(display, scene);
    SDL_RenderSetViewport(display.renderer, NULL);

    RenderScreenSpaceRectangle(
        &display,
        &tile.borderColor,
        &tile.position,
        false
    );

    SDL_RenderPresent(display.renderer);
}

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

    for (int i = 0; i < scene.walls.size; i++)
    {
        LineSegment2D* line = DLLAt(&scene.walls, i);

        RenderCameraSpaceLine(
            &scene.camera,
            &display,
            &scene.colors.wallCol,
            line->p1.x,
            line->p1.y,
            line->p2.x,
            line->p2.y
        );
    }
}

void RenderPlayer(const Display display, const Scene scene)
{
    SDL_Rect rect =
    {
        .x = scene.player.frame.position.x - (PLAYER_BASE_SIZE / 2.0),
        .y = scene.player.frame.position.y - (PLAYER_BASE_SIZE / 2.0),
        .w = PLAYER_BASE_SIZE,
        .h = PLAYER_BASE_SIZE
    };

    RenderCameraSpaceRectangle(
        &scene.camera,
        &display,
        &scene.colors.playerCol,
        &rect,
        true
    );
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

    RenderCameraSpaceLine(
        &scene.camera,
        &display,
        &scene.colors.rayCol,
        scene.player.frame.position.x,
        scene.player.frame.position.y,
        intersectedPoints[shortestIndex].x,
        intersectedPoints[shortestIndex].y
    );

    SDL_Rect rect;
    rect.x = intersectedPoints[shortestIndex].x - 2;
    rect.y = intersectedPoints[shortestIndex].y - 2;
    rect.w = 4;
    rect.h = 4;

    RenderCameraSpaceRectangle(
        &scene.camera,
        &display,
        &scene.colors.intersectCol,
        &rect,
        true
    );
}

void RenderCameraSpaceLine(
    const Frame2D* const cameraFrame, 
    const Display* const display, 
    const Color* const color, 
    const double x1, 
    const double y1, 
    const double x2, 
    const double y2)
{
    RenderScreenSpaceLine(
        display, color,
        x1 + cameraFrame->position.x,
        y1 + cameraFrame->position.y,
        x2 + cameraFrame->position.x,
        y2 + cameraFrame->position.y
    );
}

void RenderScreenSpaceLine(
    const Display* const display,
    const Color* const color,
    const double x1,
    const double y1,
    const double x2,
    const double y2)
{
    int res =
        SDL_SetRenderDrawColor(
            display->renderer,
            color->r,
            color->g,
            color->b,
            color->a
        );

    assert(res == 0);

    res =
        SDL_RenderDrawLineF(
            display->renderer,
            x1,
            y1,
            x2,
            y2
        );

    assert(res == 0);
}

void RenderCameraSpaceRectangle(
    const Frame2D* const cameraFrame, 
    const Display* const display, 
    const Color* const color, 
    const SDL_Rect* const area, 
    bool fill)
{
    SDL_Rect cameraSpaceRect =
    {
        .x = area->x + cameraFrame->position.x,
        .y = area->y + cameraFrame->position.y,
        .w = area->w,
        .h = area->h
    };

    RenderScreenSpaceRectangle(
        display,
        color,
        &cameraSpaceRect,
        fill
    );
}

void RenderScreenSpaceRectangle(
    const Display* const display,
    const Color* const color,
    const SDL_Rect* const area,
    bool fill)
{
    int res =
        SDL_SetRenderDrawColor(
            display->renderer,
            color->r,
            color->g,
            color->b,
            color->a
        );

    assert(res == 0);

    if (fill)
    {
        res =
            SDL_RenderFillRect(
                display->renderer,
                area
            );

        assert(res == 0);
    }
    else
    {
        SDL_FPoint points[5];
        points[0] = (SDL_FPoint){ .x = area->x, .y = area->y };
        points[1] = (SDL_FPoint){ .x = area->x + area->w, .y = area->y };
        points[2] = (SDL_FPoint){ .x = area->x + area->w, .y = area->y + area->h };
        points[3] = (SDL_FPoint){ .x = area->x, .y = area->y + area->h };
        points[4] = (SDL_FPoint){ .x = area->x, .y = area->y };

        res =
            SDL_RenderDrawLinesF(
                display->renderer,
                points,
                5
            );

        assert(res == 0);
    }
}
