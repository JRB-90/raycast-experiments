#include "render.h"
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <float.h>
#include "SDL.h"
#include "crayconsts.h"
#include "craymath.h"

#define MAX_INTERSECTIONS 10

// Private function defs
Frame2D CalculateTileCameraPosition(
    const Scene scene,
    const DisplayTile tile
);
void RenderTileInternal(
    const Display display, 
    const Scene scene, 
    const Frame2D cameraFrame, 
    const DisplayTile tile
);
void RenderSceneTopDownInternal(
    const Display display, 
    const Scene scene, 
    const Frame2D cameraFrame
);
void RenderSceneFirstPersonInternal(
    const Display display,
    const Scene scene,
    const int width,
    const int height
);
void RenderVerticalWallStrip(
    const Display display,
    const Scene scene,
    const int xPosition,
    const int height,
    const double distanceToWall,
    const double angleWithWall
);
void RenderWallsTopDown(
    const Display display, 
    const Scene scene, 
    const Frame2D cameraFrame
);
void RenderPlayerTopDown(
    const Display display, 
    const Scene scene, 
    const Frame2D cameraFrame
);
void RenderProjectionTopDown(
    const Display display, 
    const Scene scene, 
    const Frame2D cameraFrame
);
void RenderRayTopDown(
    const Display display, 
    const Scene scene,
    const Frame2D cameraFrame,
    const Vector2D ray
);
void ToScreenSpace(
    const Frame2D* const cameraFrame,
    const double x,
    const double y,
    double* xRes,
    double* yRes
);
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

// Public function bodies
void RenderTiles(
    const Display display, 
    const Scene scene, 
    const DisplayTile tiles[], 
    int count)
{
    uint64_t start = SDL_GetTicks64();

    ClearScreen(display, scene);

    for (int i = 0; i < count; i++)
    {
        RenderTileInternal(
            display,
            scene,
            CalculateTileCameraPosition(scene, tiles[i]),
            tiles[i]
        );
    }

    SDL_RenderPresent(display.renderer);

    uint64_t timeTaken = SDL_GetTicks64() - start;
    printf("Rendering time: %llums\n", timeTaken);
}

void RenderTile(
    const Display display,
    const Scene scene,
    const DisplayTile tile)
{
    uint64_t start = SDL_GetTicks64();

    ClearScreen(display, scene);

    RenderTileInternal(
        display,
        scene,
        CalculateTileCameraPosition(scene, tile),
        tile
    );

    SDL_RenderPresent(display.renderer);

    uint64_t timeTaken = SDL_GetTicks64() - start;
    printf("Rendering time: %llums\n", timeTaken);
}

void RenderSceneTopDown(
    const Display display, 
    const Scene scene)
{
    uint64_t start = SDL_GetTicks64();
    RenderSceneTopDownInternal(display, scene, scene.camera);
    uint64_t timeTaken = SDL_GetTicks64() - start;
    printf("Rendering time: %llums\n", timeTaken);
}

void RenderSceneFirstPerson(
    const Display display,
    const Scene scene)
{
    uint64_t start = SDL_GetTicks64();
    RenderSceneFirstPersonInternal(
        display,
        scene,
        0,
        0,
        display.width,
        display.height
    );
    uint64_t timeTaken = SDL_GetTicks64() - start;
    printf("Rendering time: %llums\n", timeTaken);
}

// Private function bodies
Frame2D CalculateTileCameraPosition(
    const Scene scene, 
    const DisplayTile tile)
{
    Frame2D cameraFrame;

    if (tile.tileType == StaticScene)
    {
        cameraFrame =
        (Frame2D){
            .position =
            {
                .x = (tile.position.w / 2),
                .y = (tile.position.h / 2)
            },
            .theta = 0.0
        };
    }
    else if (tile.tileType == StaticPlayer)
    {
        // TODO - Need to somehow figure out how to make camera right?
        double x = (tile.position.w / 2);
        double y = (tile.position.h / 2);

        cameraFrame =
        (Frame2D){
            .position =
            {
                .x = x + scene.player.frame.position.x,
                .y = y + scene.player.frame.position.y
            },
            .theta = scene.player.frame.theta
        };
    }
    else
    {
        cameraFrame =
            (Frame2D){
                .position =
                {
                    .x = (tile.position.w / 2),
                    .y = (tile.position.h / 2)
                },
                .theta = 0.0
        };
    }

    return cameraFrame;
}

void RenderTileInternal(
    const Display display,
    const Scene scene,
    const Frame2D cameraFrame,
    const DisplayTile tile)
{
    SDL_RenderSetViewport(display.renderer, &tile.position);

    if (tile.tileType == StaticPlayer ||
        tile.tileType == StaticScene)
    {
        RenderSceneTopDownInternal(
            display, 
            scene, 
            cameraFrame
        );
    }
    else if (tile.tileType == FirstPerson)
    {
        RenderSceneFirstPersonInternal(
            display,
            scene,
            tile.position.w,
            tile.position.h
        );
    }

    SDL_RenderSetViewport(display.renderer, NULL);

    RenderScreenSpaceRectangle(
        &display,
        &tile.borderColor,
        &tile.position,
        false
    );
}

void RenderSceneTopDownInternal(
    const Display display, 
    const Scene scene,
    const Frame2D cameraFrame)
{
    RenderProjectionTopDown(display, scene, cameraFrame);
    RenderWallsTopDown(display, scene, cameraFrame);
    RenderPlayerTopDown(display, scene, cameraFrame);
}

void RenderSceneFirstPersonInternal(
    const Display display, 
    const Scene scene,
    const int width,
    const int height)
{
    Vector2D worldForward = { .x = 0.0, .y = -1.0 };
    double angleInterval = (scene.player.fov * 2.0) / ((double)(width - 1));
    double startAngle = scene.player.frame.theta - scene.player.fov;

    for (int i = 0; i < width; i++)
    {
        LineSegment2D* nearestWall = NULL;
        double distanceToWall = DBL_MAX;
        Point2D wallIntersection;

        double theta = startAngle + ((double)i * angleInterval);

        Vector2D lookDir =
            FindLookVector(
                worldForward,
                theta
            );

        lookDir = Vec2DNormalise(lookDir);

        DLLNode* current = scene.walls.head;

        while (current != NULL)
        {
            double distanceToLine;
            Point2D intersectionPoint;
            LineSegment2D* line = (LineSegment2D*)current->data;

            bool doesIntersect =
                DoesRayInterectLine(
                    scene.player.frame.position,
                    lookDir,
                    *line,
                    &distanceToLine,
                    &intersectionPoint
                );

            if (doesIntersect)
            {
                if (distanceToLine < distanceToWall)
                {
                    nearestWall = line;
                    distanceToWall = distanceToLine;
                    wallIntersection = intersectionPoint;
                }
            }

            current = current->next;
        }

        if (nearestWall == NULL)
        {
            return;
        }

        Vector2D wallVector = 
            Vec2DNormalise(
                Vec2DBetween(nearestWall->p1, nearestWall->p2)
            );

        double angleWithWall = 
            Vec2DDot(
                lookDir,
                Vec2DNormalise(Vec2DBetween(nearestWall->p1, nearestWall->p2))
            );

        angleWithWall = 1.0 - (fabs(angleWithWall) / 2.0);

        RenderVerticalWallStrip(
            display,
            scene,
            i,
            height,
            distanceToWall,
            angleWithWall
        );

        if (theta > 0.0)
        {
            int temp = 0;
            temp++;
        }
    }
}

void RenderVerticalWallStrip(
    const Display display,
    const Scene scene,
    const int xPosition,
    const int height,
    const double distanceToWall,
    const double angleWithWall)
{
    double h = tan(ToRad(scene.player.fov)) * distanceToWall;
    double wallHeightPixels = WALL_HEIGHT / h;
    double wallStartY = (height / 2.0) - (wallHeightPixels / 2.0);
    double wallEndY = (height / 2.0) + (wallHeightPixels / 2.0);

    int res =
        SDL_SetRenderDrawColor(
            display.renderer,
            scene.colors.ceilingColor.r,
            scene.colors.ceilingColor.g,
            scene.colors.ceilingColor.b,
            255
        );

    assert(res == 0);

    for (int i = 0; i < wallStartY; i++)
    {
        res =
            SDL_RenderDrawPoint(
                display.renderer,
                xPosition,
                i
            );

        assert(res == 0);
    }

    res =
        SDL_SetRenderDrawColor(
            display.renderer,
            scene.colors.wallCol.r * angleWithWall,
            scene.colors.wallCol.g * angleWithWall,
            scene.colors.wallCol.b * angleWithWall,
            255
        );

    assert(res == 0);

    for (int i = wallStartY; i < wallEndY; i++)
    {
        res =
            SDL_RenderDrawPoint(
                display.renderer,
                xPosition,
                i
            );

        assert(res == 0);
    }

    res =
        SDL_SetRenderDrawColor(
            display.renderer,
            scene.colors.floorCol.r,
            scene.colors.floorCol.g,
            scene.colors.floorCol.b,
            255
        );

    assert(res == 0);

    for (int i = wallEndY; i < height; i++)
    {
        res =
            SDL_RenderDrawPoint(
                display.renderer,
                xPosition,
                i
            );

        assert(res == 0);
    }
}

void ClearScreen(
    const Display display,
    const Scene scene)
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

void RenderWallsTopDown(
    const Display display, 
    const Scene scene, 
    const Frame2D cameraFrame)
{
    if (scene.walls.size < 1)
    {
        return;
    }

    for (int i = 0; i < scene.walls.size; i++)
    {
        LineSegment2D* line = DLLAt(&scene.walls, i);

        RenderCameraSpaceLine(
            &cameraFrame,
            &display,
            &scene.colors.wallCol,
            line->p1.x,
            line->p1.y,
            line->p2.x,
            line->p2.y
        );
    }
}

void RenderPlayerTopDown(
    const Display display, 
    const Scene scene, 
    const Frame2D cameraFrame)
{
    SDL_Rect rect =
    {
        .x = scene.player.frame.position.x - (PLAYER_BASE_SIZE / 2.0),
        .y = scene.player.frame.position.y - (PLAYER_BASE_SIZE / 2.0),
        .w = PLAYER_BASE_SIZE,
        .h = PLAYER_BASE_SIZE
    };

    RenderCameraSpaceRectangle(
        &cameraFrame,
        &display,
        &scene.colors.playerCol,
        &rect,
        true
    );
}

void RenderProjectionTopDown(
    const Display display, 
    const Scene scene, 
    const Frame2D cameraFrame)
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

        RenderRayTopDown(
            display,
            scene,
            cameraFrame,
            Vec2DNormalise(lookDir)
        );
    }
}

void RenderRayTopDown(
    const Display display, 
    const Scene scene, 
    const Frame2D cameraFrame, 
    const Vector2D ray)
{
    LineSegment2D* nearestWall = NULL;
    double distanceToWall = DBL_MAX;
    Point2D wallIntersection;
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
            if (distanceToLine < distanceToWall)
            {
                nearestWall = line;
                distanceToWall = distanceToLine;
                wallIntersection = intersectionPoint;
            }
        }

        current = current->next;
    }

    if (nearestWall == 0)
    {
        return;
    }

    RenderCameraSpaceLine(
        &cameraFrame,
        &display,
        &scene.colors.rayCol,
        scene.player.frame.position.x,
        scene.player.frame.position.y,
        wallIntersection.x,
        wallIntersection.y
    );

    SDL_Rect rect;
    rect.x = wallIntersection.x - 2;
    rect.y = wallIntersection.y - 2;
    rect.w = 4;
    rect.h = 4;

    RenderCameraSpaceRectangle(
        &cameraFrame,
        &display,
        &scene.colors.intersectCol,
        &rect,
        true
    );
}

void ToScreenSpace(
    const Frame2D* const cameraFrame,
    const double x, 
    const double y,
    double* xRes, 
    double* yRes)
{
    // TODO - May need to player position at this point..

    double t = ToRad(-cameraFrame->theta);
    double s = sin(t);
    double c = cos(t);

    double xr = (x * c) - (y * s) + cameraFrame->position.x;
    double yr = (x * s) + (y * c) + cameraFrame->position.y;

    *xRes = xr;
    *yRes = yr;
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
    double x1Screen;
    double y1Screen;
    double x2Screen;
    double y2Screen;

    ToScreenSpace(
        cameraFrame,
        x1,
        y1,
        &x1Screen,
        &y1Screen
    );

    ToScreenSpace(
        cameraFrame,
        x2,
        y2,
        &x2Screen,
        &y2Screen
    );

    RenderScreenSpaceLine(
        display, 
        color,
        x1Screen,
        y1Screen,
        x2Screen,
        y2Screen
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
    double x1Screen;
    double y1Screen;

    ToScreenSpace(
        cameraFrame,
        area->x,
        area->y,
        &x1Screen,
        &y1Screen
    );

    SDL_Rect cameraSpaceRect =
    {
        .x = x1Screen,
        .y = y1Screen,
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
