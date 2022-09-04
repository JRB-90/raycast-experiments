#include "crrender.h"
#include <assert.h>
#include <stdio.h>
#include <float.h>
#include "crconsts.h"
#include "crmath.h"
#include "crtime.h"
#include "crdraw.h"

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
    const DisplayTile* const tile,
    CycleProfile* profile
);
void RenderSceneTopDownInternal(
    const Display* const display,
    const Rect* const viewport,
    const Scene* const scene,
    const Frame2D* const cameraFrame,
    CycleProfile* profile
);
void RenderSceneFirstPersonInternal(
    const Display* const display,
    const Rect* const viewport,
    const Scene* const scene,
    const int width,
    const int height,
    CycleProfile* profile
);
void RenderVerticalWallStrip(
    const Display* const display,
    const Rect* const viewport,
    const Scene* const scene,
    const int xPosition,
    const int height,
    const double distanceToWall,
    const double angleWithWall,
    CycleProfile* profile
);
void RenderWallsTopDown(
    const Display* const display,
    const Rect* const viewport,
    const Scene* const scene,
    const Frame2D* const cameraFrame,
    CycleProfile* profile
);
void RenderPlayerTopDown(
    const Display* const display,
    const Rect* const viewport,
    const Scene* const scene,
    const Frame2D* const cameraFrame,
    CycleProfile* profile
);
void RenderProjectionTopDown(
    const Display* const display,
    const Rect* const viewport,
    const Scene* const scene,
    const Frame2D* const cameraFrame,
    CycleProfile* profile
);
void RenderRayTopDown(
    const Display* const display,
    const Rect* const viewport,
    const Scene* const scene,
    const Frame2D* const cameraFrame,
    const Vector2D* const ray,
    CycleProfile* profile
);
void ToScreenSpace(
    const Scene* const scene,
    const Frame2D* const cameraFrame,
    const double x,
    const double y,
    int* xRes,
    int* yRes
);
void RenderCameraSpaceLine(
    const Display* const display,
    const Rect* const viewport,
    const Scene* const scene,
    const Frame2D* const cameraFrame,
    const Color* const color,
    const double x1,
    const double y1,
    const double x2,
    const double y2
);
void RenderCameraSpaceRectangle(
    const Display* const display,
    const Rect* const viewport,
    const Scene* const scene,
    const Frame2D* const cameraFrame,
    const Color* const color,
    const Rect* const area,
    bool fill
);

#pragma endregion

#pragma region Public Function Bodies

// Public function bodies
void RenderTiles(
    const Display* const display,
    const Scene* const scene,
    const DisplayTile tiles[],
    int count,
    CycleProfile* const profile)
{
    ClearScreen(display, scene, profile);

    for (int i = 0; i < count; i++)
    {
        DisplayTile tile = tiles[i];
        Frame2D tileCamPos = CalculateTileCameraPosition(scene, &tile);
        RenderTileInternal(
            display,
            scene,
            &tileCamPos,
            &tile,
            profile
        );
    }

    RenderDisplay(display, profile);
}

void RenderTile(
    const Display* const display,
    const Scene* const scene,
    const DisplayTile* const tile,
    CycleProfile* const profile)
{
    Frame2D tileCamPos = CalculateTileCameraPosition(scene, tile);

    ClearScreen(display, scene, profile);
    RenderTileInternal(
        display,
        scene,
        &tileCamPos,
        tile,
        profile
    );
    RenderDisplay(display, profile);
}

void RenderSceneTopDown(
    const Display* const display,
    const Scene* const scene,
    CycleProfile* const profile)
{
    Rect viewport =
    {
        .x = 0,
        .y = 0,
        .w = display->width,
        .h = display->height
    };

    ClearScreen(display, scene, profile);
    RenderSceneTopDownInternal(
        display,
        &viewport,
        scene, 
        &scene->camera, 
        profile
    );
    RenderDisplay(display, profile);
}

void RenderSceneFirstPerson(
    const Display* const display,
    const Scene* const scene,
    CycleProfile* const profile)
{
    Rect viewport =
    {
        .x = 0,
        .y = 0,
        .w = display->width,
        .h = display->height
    };

    RenderSceneFirstPersonInternal(
        display,
        &viewport,
        scene,
        display->width,
        display->height,
        profile
    );
    
    RenderDisplay(display, profile);
}

void ClearScreen(
    const Display* const display,
    const Scene* const scene,
    CycleProfile* const profile)
{
    uint64_t startTime = GetTicks();
    DrawClearColor(display, &scene->colors.clearCol);
    profile->clearTimeMS = GetTimeInMS(GetTicks() - startTime);
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
                .x = (tile->viewport.w / 2),
                .y = (tile->viewport.h / 2)
            },
            .theta = 0.0
        };
    }
    else if (tile->tileType == StaticPlayer)
    {
        // TODO - Need to somehow figure out how to make camera right?
        double x = (tile->viewport.w / 2.0);
        double y = (tile->viewport.h / 2.0);

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
                    .x = (tile->viewport.w / 2),
                    .y = (tile->viewport.h / 2)
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
    const DisplayTile* const tile,
    CycleProfile* profile)
{
    uint64_t startTime = GetTicks();

    if (tile->tileType == StaticPlayer ||
        tile->tileType == StaticScene)
    {
        RenderSceneTopDownInternal(
            display,
            &tile->viewport,
            scene, 
            cameraFrame,
            profile
        );
    }
    else if (tile->tileType == FirstPerson)
    {
        RenderSceneFirstPersonInternal(
            display,
            &tile->viewport,
            scene,
            tile->viewport.w,
            tile->viewport.h,
            profile
        );
    }

    DrawRect(
        display,
        &tile->viewport,
        &tile->borderColor,
        0,
        0,
        tile->viewport.w,
        tile->viewport.h
    );

    AddSample(&profile->tileRender, GetTicks() - startTime);
}

void RenderSceneTopDownInternal(
    const Display* const display,
    const Rect* const viewport,
    const Scene* const scene,
    const Frame2D* const cameraFrame,
    CycleProfile* profile)
{
    uint64_t startTime = GetTicks();
    RenderProjectionTopDown(display, viewport, scene, cameraFrame, profile);
    RenderWallsTopDown(display, viewport, scene, cameraFrame, profile);
    RenderPlayerTopDown(display, viewport, scene, cameraFrame, profile);
    AddSample(&profile->topRender, GetTicks() - startTime);
}

void RenderSceneFirstPersonInternal(
    const Display* const display,
    const Rect* const viewport,
    const Scene* const scene,
    const int width,
    const int height,
    CycleProfile* profile)
{
    uint64_t firstStartTime = GetTicks();

    Vector2D worldForward = { .x = 0.0, .y = -1.0 };
    double angleInterval = (scene->player.fov * 2.0) / ((double)(width - 1));
    double startAngle = scene->player.frame.theta - scene->player.fov;

    for (int i = 0; i < width; i++)
    {
        uint64_t vertStartTime = GetTicks();

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
                viewport,
                scene,
                i,
                height,
                0.0,
                0.0,
                profile
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
            viewport,
            scene,
            i,
            height,
            distanceToWall,
            angleWithWall,
            profile
        );

        AddSample(&profile->vertRender, GetTicks() - vertStartTime);
    }

    AddSample(&profile->firstRender, GetTicks() - firstStartTime);
}

void RenderVerticalWallStrip(
    const Display* const display,
    const Rect* const viewport,
    const Scene* const scene,
    const int xPosition,
    const int height,
    const double distanceToWall,
    const double angleWithWall,
    CycleProfile* profile)
{
    double wallHeightPixels = 0.0;

    if (distanceToWall > 0.0)
    {
        double h = tan(ToRad(scene->player.fov)) * distanceToWall;
        wallHeightPixels = WALL_HEIGHT / h;
    }
    
    int wallStartY = (int)((height / 2.0) - (wallHeightPixels / 2.0));
    int wallEndY = (int)((height / 2.0) + (wallHeightPixels / 2.0));

    for (int i = 0; i < wallStartY; i++)
    {
        WritePixelViewport(
            display,
            viewport,
            &scene->colors.ceilingColor,
            xPosition,
            i
        );
    }

    Color wallColor =
    {
        .r = (uint8_t)(scene->colors.wallCol.r * angleWithWall),
        .g = (uint8_t)(scene->colors.wallCol.g * angleWithWall),
        .b = (uint8_t)(scene->colors.wallCol.b * angleWithWall),
        .a = 255
    };

    for (int i = wallStartY; i < wallEndY; i++)
    {
        WritePixelViewport(
            display,
            viewport,
            &wallColor,
            xPosition,
            i
        );
    }

    for (int i = wallEndY; i < height; i++)
    {
        WritePixelViewport(
            display,
            viewport,
            &scene->colors.floorCol,
            xPosition,
            i
        );
    }
}

void RenderWallsTopDown(
    const Display* const display,
    const Rect* const viewport,
    const Scene* const scene,
    const Frame2D* const cameraFrame,
    CycleProfile* profile)
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
            viewport,
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
    const Rect* const viewport,
    const Scene* const scene,
    const Frame2D* const cameraFrame,
    CycleProfile* profile)
{
    Rect rect =
    {
        .x = (int)(scene->player.frame.position.x - (PLAYER_BASE_SIZE / 2.0)),
        .y = (int)(scene->player.frame.position.y - (PLAYER_BASE_SIZE / 2.0)),
        .w = (int)PLAYER_BASE_SIZE,
        .h = (int)PLAYER_BASE_SIZE
    };

    RenderCameraSpaceRectangle(
        display,
        viewport,
        scene,
        cameraFrame,
        &scene->colors.playerCol,
        &rect,
        true
    );
}

void RenderProjectionTopDown(
    const Display* const display,
    const Rect* const viewport,
    const Scene* const scene,
    const Frame2D* const cameraFrame,
    CycleProfile* profile)
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
            viewport,
            scene,
            cameraFrame,
            &lookDir,
            profile
        );
    }
}

void RenderRayTopDown(
    const Display* const display,
    const Rect* const viewport,
    const Scene* const scene,
    const Frame2D* const cameraFrame,
    const Vector2D* const ray,
    CycleProfile* profile)
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
        viewport,
        scene,
        cameraFrame,
        &scene->colors.rayCol,
        scene->player.frame.position.x,
        scene->player.frame.position.y,
        wallIntersection.x,
        wallIntersection.y
    );

    Rect rect =
    {
        .x = (int)(wallIntersection.x - 2.0),
        .y = (int)(wallIntersection.y - 2.0),
        .w = 4,
        .h = 4
    };

    RenderCameraSpaceRectangle(
        display,
        viewport,
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
    int* xRes, 
    int* yRes)
{
    double t = ToRad(-cameraFrame->theta);
    double s = sin(t);
    double c = cos(t);

    int xr = (int)((x * c) - (y * s) + cameraFrame->position.x);
    int yr = (int)((x * s) + (y * c) + cameraFrame->position.y);

    *xRes = xr;
    *yRes = yr;
}

void RenderCameraSpaceLine(
    const Display* const display,
    const Rect* const viewport,
    const Scene* const scene,
    const Frame2D* const cameraFrame,
    const Color* const color, 
    const double x1,
    const double y1, 
    const double x2, 
    const double y2)
{
    int x1Screen;
    int y1Screen;
    int x2Screen;
    int y2Screen;

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

    DrawLine(
        display, 
        viewport,
        color,
        x1Screen,
        y1Screen,
        x2Screen,
        y2Screen
    );
}

void RenderCameraSpaceRectangle(
    const Display* const display,
    const Rect* const viewport,
    const Scene* const scene,
    const Frame2D* const cameraFrame, 
    const Color* const color, 
    const Rect* const area,
    bool fill)
{
    int x1Screen = 0;
    int y1Screen = 0;

    ToScreenSpace(
        scene,
        cameraFrame,
        area->x,
        area->y,
        &x1Screen,
        &y1Screen
    );

    if (fill)
    {
        DrawRectFilled(
            display,
            viewport,
            color,
            x1Screen,
            y1Screen,
            area->w,
            area->h
        );
    }
    else
    {
        DrawRect(
            display,
            viewport,
            color,
            x1Screen,
            y1Screen,
            area->w,
            area->h
        );
    }
}

#pragma endregion
