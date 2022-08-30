#include "render.h"
#include <assert.h>
#include <stdio.h>
#include <float.h>
#include "SDL.h"
#include "crayconsts.h"
#include "craymath.h"

#pragma region Private Function Definitions

// Private function defs
Frame2D CalculateTileCameraPosition(
    const Scene* const scene,
    const DisplayTile* const tile
);
void RenderTileInternal(
    const Display* const display,
    const Scene* const scene,
    const Frame2D* const cameraFrame,
    const DisplayTile* const tile
);
void RenderSceneTopDownInternal(
    const Display* const display,
    const Scene* const scene,
    const Frame2D* const cameraFrame
);
void RenderSceneFirstPersonInternal(
    const Display* const display,
    const Scene* const scene,
    const int width,
    const int height
);
void RenderVerticalWallStrip(
    const Display* const display,
    const Scene* const scene,
    const int xPosition,
    const int height,
    const double distanceToWall,
    const double angleWithWall
);
void RenderWallsTopDown(
    const Display* const display,
    const Scene* const scene,
    const Frame2D* const cameraFrame
);
void RenderPlayerTopDown(
    const Display* const display,
    const Scene* const scene,
    const Frame2D* const cameraFrame
);
void RenderProjectionTopDown(
    const Display* const display,
    const Scene* const scene,
    const Frame2D* const cameraFrame
);
void RenderRayTopDown(
    const Display* const display,
    const Scene* const scene,
    const Frame2D* const cameraFrame,
    const Vector2D* const ray
);
void ToScreenSpace(
    const Scene* const scene,
    const Frame2D* const cameraFrame,
    const double x,
    const double y,
    double* xRes,
    double* yRes
);
void RenderCameraSpaceLine(
    const Display* const display,
    const Scene* const scene,
    const Frame2D* const cameraFrame,
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
    const Display* const display,
    const Scene* const scene,
    const Frame2D* const cameraFrame,
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

#pragma endregion

#pragma region Public Function Bodies

// Public function bodies
void RenderTiles(
    const Display* const display,
    const Scene* const scene,
    const DisplayTile tiles[],
    int count)
{
    ClearScreen(display, scene);

    for (int i = 0; i < count; i++)
    {
        DisplayTile tile = tiles[i];
        Frame2D tileCamPos = CalculateTileCameraPosition(scene, &tile);

        RenderTileInternal(
            display,
            scene,
            &tileCamPos,
            &tile
        );
    }

    SDL_RenderPresent(display->renderer);
}

void RenderTile(
    const Display* const display,
    const Scene* const scene,
    const DisplayTile* const tile)
{
    ClearScreen(display, scene);

    Frame2D tileCamPos = CalculateTileCameraPosition(scene, tile);

    RenderTileInternal(
        display,
        scene,
        &tileCamPos,
        tile
    );

    SDL_RenderPresent(display->renderer);
}

void RenderSceneTopDown(
    const Display* const display,
    const Scene* const scene)
{
    SDL_RenderSetViewport(display->renderer, NULL);
    ClearScreen(display, scene);
    RenderSceneTopDownInternal(display, scene, &scene->camera);
    SDL_RenderPresent(display->renderer);
}

void RenderSceneFirstPerson(
    const Display* const display,
    const Scene* const scene)
{
    SDL_RenderSetViewport(display->renderer, NULL);
    ClearScreen(display, scene);
    RenderSceneFirstPersonInternal(
        display,
        scene,
        display->width,
        display->height
    );
    SDL_RenderPresent(display->renderer);
}

void ClearScreen(
    const Display* const display,
    const Scene* const scene)
{
    int res =
        SDL_SetRenderDrawColor(
            display->renderer,
            scene->colors.clearCol.r,
            scene->colors.clearCol.g,
            scene->colors.clearCol.b,
            scene->colors.clearCol.a
        );

    assert(res == 0);

    res = SDL_RenderClear(display->renderer);

    assert(res == 0);
}

#pragma endregion

#pragma region Private Function Bodies

// Private function bodies
Frame2D CalculateTileCameraPosition(
    const Scene* const scene,
    const DisplayTile* const tile)
{
    Frame2D cameraFrame;

    if (tile->tileType == StaticScene)
    {
        cameraFrame =
        (Frame2D){
            .position =
            {
                .x = (tile->position.w / 2),
                .y = (tile->position.h / 2)
            },
            .theta = 0.0
        };
    }
    else if (tile->tileType == StaticPlayer)
    {
        // TODO - Need to somehow figure out how to make camera right?
        double x = (tile->position.w / 2.0);
        double y = (tile->position.h / 2.0);

        cameraFrame =
        (Frame2D){
            .position =
            {
                .x = x + scene->player.frame.position.x,
                .y = y + scene->player.frame.position.y
            },
            .theta = scene->player.frame.theta
        };
    }
    else
    {
        cameraFrame =
            (Frame2D){
                .position =
                {
                    .x = (tile->position.w / 2),
                    .y = (tile->position.h / 2)
                },
                .theta = 0.0
        };
    }

    return cameraFrame;
}

void RenderTileInternal(
    const Display* const display,
    const Scene* const scene,
    const Frame2D* const cameraFrame,
    const DisplayTile* const tile)
{
    SDL_RenderSetViewport(display->renderer, &tile->position);

    if (tile->tileType == StaticPlayer ||
        tile->tileType == StaticScene)
    {
        RenderSceneTopDownInternal(
            display, 
            scene, 
            cameraFrame
        );
    }
    else if (tile->tileType == FirstPerson)
    {
        RenderSceneFirstPersonInternal(
            display,
            scene,
            tile->position.w,
            tile->position.h
        );
    }

    SDL_RenderSetViewport(display->renderer, NULL);

    RenderScreenSpaceRectangle(
        display,
        &tile->borderColor,
        &tile->position,
        false
    );
}

void RenderSceneTopDownInternal(
    const Display* const display,
    const Scene* const scene,
    const Frame2D* const cameraFrame)
{
    RenderProjectionTopDown(display, scene, cameraFrame);
    RenderWallsTopDown(display, scene, cameraFrame);
    RenderPlayerTopDown(display, scene, cameraFrame);
}

void RenderSceneFirstPersonInternal(
    const Display* const display,
    const Scene* const scene,
    const int width,
    const int height)
{
    Vector2D worldForward = { .x = 0.0, .y = -1.0 };
    double angleInterval = (scene->player.fov * 2.0) / ((double)(width - 1));
    double startAngle = scene->player.frame.theta - scene->player.fov;

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

        DLLNode* current = scene->walls.head;

        while (current != NULL)
        {
            double distanceToLine;
            Point2D intersectionPoint;
            LineSegment2D* line = (LineSegment2D*)current->data;

            bool doesIntersect =
                DoesRayInterectLine(
                    scene->player.frame.position,
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
            RenderVerticalWallStrip(
                display,
                scene,
                i,
                height,
                0.0,
                0.0
            );

            continue;
        }

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
    }
}

void RenderVerticalWallStrip(
    const Display* const display,
    const Scene* const scene,
    const int xPosition,
    const int height,
    const double distanceToWall,
    const double angleWithWall)
{
    double wallHeightPixels = 0.0;

    if (distanceToWall > 0.0)
    {
        double h = tan(ToRad(scene->player.fov)) * distanceToWall;
        wallHeightPixels = WALL_HEIGHT / h;
    }
    
    int wallStartY = (int)((height / 2.0) - (wallHeightPixels / 2.0));
    int wallEndY = (int)((height / 2.0) + (wallHeightPixels / 2.0));

    int res =
        SDL_SetRenderDrawColor(
            display->renderer,
            scene->colors.ceilingColor.r,
            scene->colors.ceilingColor.g,
            scene->colors.ceilingColor.b,
            255
        );

    assert(res == 0);

    for (int i = 0; i < wallStartY; i++)
    {
        res =
            SDL_RenderDrawPoint(
                display->renderer,
                xPosition,
                i
            );

        assert(res == 0);
    }

    res =
        SDL_SetRenderDrawColor(
            display->renderer,
            (uint8_t)(scene->colors.wallCol.r * angleWithWall),
            (uint8_t)(scene->colors.wallCol.g * angleWithWall),
            (uint8_t)(scene->colors.wallCol.b * angleWithWall),
            255
        );

    assert(res == 0);

    for (int i = wallStartY; i < wallEndY; i++)
    {
        res =
            SDL_RenderDrawPoint(
                display->renderer,
                xPosition,
                i
            );

        assert(res == 0);
    }

    res =
        SDL_SetRenderDrawColor(
            display->renderer,
            scene->colors.floorCol.r,
            scene->colors.floorCol.g,
            scene->colors.floorCol.b,
            255
        );

    assert(res == 0);

    for (int i = wallEndY; i < height; i++)
    {
        res =
            SDL_RenderDrawPoint(
                display->renderer,
                xPosition,
                i
            );

        assert(res == 0);
    }
}

void RenderWallsTopDown(
    const Display* const display,
    const Scene* const scene,
    const Frame2D* const cameraFrame)
{
    if (scene->walls.size < 1)
    {
        return;
    }

    for (uint32_t i = 0; i < scene->walls.size; i++)
    {
        LineSegment2D* line = DLLAt((DLList* const)&scene->walls, i);

        RenderCameraSpaceLine(
            display,
            scene,
            cameraFrame,
            &scene->colors.wallCol,
            line->p1.x,
            line->p1.y,
            line->p2.x,
            line->p2.y
        );
    }
}

void RenderPlayerTopDown(
    const Display* const display,
    const Scene* const scene,
    const Frame2D* const cameraFrame)
{
    SDL_Rect rect =
    {
        .x = (int)(scene->player.frame.position.x - (PLAYER_BASE_SIZE / 2.0)),
        .y = (int)(scene->player.frame.position.y - (PLAYER_BASE_SIZE / 2.0)),
        .w = (int)PLAYER_BASE_SIZE,
        .h = (int)PLAYER_BASE_SIZE
    };

    RenderCameraSpaceRectangle(
        display,
        scene,
        cameraFrame,
        &scene->colors.playerCol,
        &rect,
        true
    );
}

void RenderProjectionTopDown(
    const Display* const display,
    const Scene* const scene,
    const Frame2D* const cameraFrame)
{
    Vector2D worldForward = { .x = 0.0, .y = -1.0 };
    int widthIncrements = 9;
    double angleInterval = (scene->player.fov * 2.0) / ((double)(widthIncrements - 1));
    double startAngle = scene->player.frame.theta - scene->player.fov;

    for (int i = 0; i < widthIncrements; i++)
    {
        double theta = startAngle + (i * angleInterval);
        
        Vector2D lookDir =
            FindLookVector(
                worldForward,
                theta
            );

        lookDir = Vec2DNormalise(lookDir);

        RenderRayTopDown(
            display,
            scene,
            cameraFrame,
            &lookDir
        );
    }
}

void RenderRayTopDown(
    const Display* const display,
    const Scene* const scene,
    const Frame2D* const cameraFrame,
    const Vector2D* const ray)
{
    const LineSegment2D* nearestWall = NULL;
    double distanceToWall = DBL_MAX;
    Point2D wallIntersection;
    DLLNode* current = scene->walls.head;

    while (current != NULL)
    {
        const LineSegment2D* const line = (LineSegment2D*)current->data;
        Point2D intersectionPoint;
        double distanceToLine;

        bool doesIntersect =
            DoesRayInterectLine(
                scene->player.frame.position,
                *ray,
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
        display,
        scene,
        cameraFrame,
        &scene->colors.rayCol,
        scene->player.frame.position.x,
        scene->player.frame.position.y,
        wallIntersection.x,
        wallIntersection.y
    );

    SDL_Rect rect =
    {
        .x = (int)(wallIntersection.x - 2.0),
        .y = (int)(wallIntersection.y - 2.0),
        .w = 4,
        .h = 4
    };

    RenderCameraSpaceRectangle(
        display,
        scene,
        cameraFrame,
        &scene->colors.intersectCol,
        &rect,
        true
    );
}

void ToScreenSpace(
    const Scene* const scene,
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
    const Display* const display,
    const Scene* const scene,
    const Frame2D* const cameraFrame,
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
        scene,
        cameraFrame,
        x1,
        y1,
        &x1Screen,
        &y1Screen
    );

    ToScreenSpace(
        scene,
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
            (float)x1,
            (float)y1,
            (float)x2,
            (float)y2
        );

    assert(res == 0);
}

void RenderCameraSpaceRectangle(
    const Display* const display,
    const Scene* const scene,
    const Frame2D* const cameraFrame, 
    const Color* const color, 
    const SDL_Rect* const area, 
    bool fill)
{
    double x1Screen;
    double y1Screen;

    ToScreenSpace(
        scene,
        cameraFrame,
        area->x,
        area->y,
        &x1Screen,
        &y1Screen
    );

    SDL_Rect cameraSpaceRect =
    {
        .x = (int)x1Screen,
        .y = (int)y1Screen,
        .w = (int)area->w,
        .h = (int)area->h
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
        SDL_Point points[5] =
        {
            (SDL_Point){ .x = area->x, .y = area->y },
            (SDL_Point){ .x = area->x + area->w, .y = area->y },
            (SDL_Point){ .x = area->x + area->w, .y = area->y + area->h },
            (SDL_Point){ .x = area->x, .y = area->y + area->h },
            (SDL_Point){ .x = area->x, .y = area->y }
        };

        res =
            SDL_RenderDrawLines(
                display->renderer,
                points,
                5
            );

        assert(res == 0);
    }
}

#pragma endregion
