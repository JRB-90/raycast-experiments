#define _USE_MATH_DEFINES

#include "crrender.h"
#include <assert.h>
#include <stdio.h>
#include <float.h>
#include <math.h>
#include "crtypes.h"
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
    const ScreenBuffer* const screen,
    const Scene* const scene,
    const Frame2D* const cameraFrame,
    const DisplayTile* const tile,
    CycleProfile* profile
);
void RenderSceneTopDownInternal(
    const ScreenBuffer* const screen,
    const Rect* const viewport,
    const Scene* const scene,
    const Frame2D* const cameraFrame,
    CycleProfile* profile
);
void RenderSceneFirstPersonInternal(
    const ScreenBuffer* const screen,
    const Rect* const viewport,
    const Scene* const scene,
    const int width,
    const int height,
    CycleProfile* profile
);
void RenderVerticalWallStrip(
    const ScreenBuffer* const screen,
    const Rect* const viewport,
    const Scene* const scene,
    const int xPosition,
    const int height,
    const float distanceToWall,
    const float angleWithWall,
    CycleProfile* profile
);
void RenderWallsTopDown(
    const ScreenBuffer* const screen,
    const Rect* const viewport,
    const Scene* const scene,
    const Frame2D* const cameraFrame,
    CycleProfile* profile
);
void RenderPlayerTopDown(
    const ScreenBuffer* const screen,
    const Rect* const viewport,
    const Scene* const scene,
    const Frame2D* const cameraFrame,
    CycleProfile* profile
);
void RenderProjectionTopDown(
    const ScreenBuffer* const screen,
    const Rect* const viewport,
    const Scene* const scene,
    const Frame2D* const cameraFrame,
    CycleProfile* profile
);
void RenderRayTopDown(
    const ScreenBuffer* const screen,
    const Rect* const viewport,
    const Scene* const scene,
    const Frame2D* const cameraFrame,
    const Vector2D* const ray,
    CycleProfile* profile
);
inline void ToScreenSpace(
    const Scene* const scene,
    const Frame2D* const cameraFrame,
    const float x,
    const float y,
    int* xRes,
    int* yRes
);
inline void RenderCameraSpaceLine(
    const ScreenBuffer* const screen,
    const Rect* const viewport,
    const Scene* const scene,
    const Frame2D* const cameraFrame,
    const Color* const color,
    const float x1,
    const float y1,
    const float x2,
    const float y2
);
inline void RenderCameraSpaceRectangle(
    const ScreenBuffer* const screen,
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
    const ScreenBuffer* const screen,
    const Scene* const scene,
    const DisplayTile tiles[],
    int count,
    CycleProfile* const profile)
{
    ClearScreen(screen, scene, profile);

    for (int i = 0; i < count; i++)
    {
        DisplayTile tile = tiles[i];
        Frame2D tileCamPos = CalculateTileCameraPosition(scene, &tile);
        RenderTileInternal(
            screen,
            scene,
            &tileCamPos,
            &tile,
            profile
        );
    }
}

void RenderTile(
    const ScreenBuffer* const screen,
    const Scene* const scene,
    const DisplayTile* const tile,
    CycleProfile* const profile)
{
    Frame2D tileCamPos = CalculateTileCameraPosition(scene, tile);

    ClearScreen(screen, scene, profile);
    RenderTileInternal(
        screen,
        scene,
        &tileCamPos,
        tile,
        profile
    );
}

void RenderSceneTopDown(
    const ScreenBuffer* const screen,
    const Scene* const scene,
    CycleProfile* const profile)
{
    Rect viewport =
    {
        .x = 0,
        .y = 0,
        .w = screen->width,
        .h = screen->height
    };

    ClearScreen(screen, scene, profile);
    RenderSceneTopDownInternal(
        screen,
        &viewport,
        scene, 
        &scene->camera, 
        profile
    );
}

void RenderSceneFirstPerson(
    const ScreenBuffer* const screen,
    const Scene* const scene,
    CycleProfile* const profile)
{
    Rect viewport =
    {
        .x = 0,
        .y = 0,
        .w = screen->width,
        .h = screen->height
    };

    RenderSceneFirstPersonInternal(
        screen,
        &viewport,
        scene,
        screen->width,
        screen->height,
        profile
    );
}

void ClearScreen(
    const ScreenBuffer* const screen,
    const Scene* const scene,
    CycleProfile* const profile)
{
    uint64_t startTime = GetTicks();
    DrawClearColor(screen, &scene->colors.clearCol);
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
            .theta = 0.0f
        };
    }
    else if (tile->tileType == StaticPlayer)
    {
        // TODO - Need to somehow figure out how to make camera right?
        float x = (tile->viewport.w / 2.0f);
        float y = (tile->viewport.h / 2.0f);

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
                .theta = 0.0f
        };
    }

    return cameraFrame;
}

void RenderTileInternal(
    const ScreenBuffer* const screen,
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
            screen,
            &tile->viewport,
            scene, 
            cameraFrame,
            profile
        );
    }
    else if (tile->tileType == FirstPerson)
    {
        RenderSceneFirstPersonInternal(
            screen,
            &tile->viewport,
            scene,
            tile->viewport.w,
            tile->viewport.h,
            profile
        );
    }

    DrawRect(
        screen,
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
    const ScreenBuffer* const screen,
    const Rect* const viewport,
    const Scene* const scene,
    const Frame2D* const cameraFrame,
    CycleProfile* profile)
{
    uint64_t startTime = GetTicks();
    RenderProjectionTopDown(screen, viewport, scene, cameraFrame, profile);
    RenderWallsTopDown(screen, viewport, scene, cameraFrame, profile);
    RenderPlayerTopDown(screen, viewport, scene, cameraFrame, profile);
    AddSample(&profile->topRender, GetTicks() - startTime);
}

void RenderSceneFirstPersonInternal(
    const ScreenBuffer* const screen,
    const Rect* const viewport,
    const Scene* const scene,
    const int width,
    const int height,
    CycleProfile* profile)
{
    uint64_t firstStartTime = GetTicks();

    Vector2D worldForward = { .x = 0.0f, .y = -1.0f };
    float angleInterval = (scene->player.settings.fov * 2.0f) / ((float)(width - 1));
    float startAngle = scene->player.frame.theta - scene->player.settings.fov;

    for (int i = 0; i < width; i++)
    {
        uint64_t vertStartTime = GetTicks();

        LineSegment2D* nearestWall = NULL;
        float distanceToWall = DBL_MAX;
        float theta = startAngle + ((float)i * angleInterval);

        Vector2D lookDir =
            FindLookVector(
                &worldForward,
                theta
            );

        DLLNode* current = scene->walls.head;

        while (current != NULL)
        {
            float distanceToLine;
            Point2D intersectionPoint;
            LineSegment2D* line = (LineSegment2D*)current->data;

            bool doesIntersect =
                DoesRayIntersectLine(
                    &scene->player.frame.position,
                    &lookDir,
                    line,
                    &distanceToLine,
                    &intersectionPoint
                );

            if (doesIntersect)
            {
                if (distanceToLine < distanceToWall)
                {
                    nearestWall = line;
                    distanceToWall = distanceToLine;
                }
            }

            current = current->next;
        }

        if (nearestWall == NULL)
        {
            RenderVerticalWallStrip(
                screen,
                viewport,
                scene,
                i,
                height,
                0.0f,
                0.0f,
                profile
            );

            continue;
        }

        float angleWithWall = 
            Vec2DDot(
                lookDir,
                Vec2DNormalise(Vec2DBetween(nearestWall->p1, nearestWall->p2))
            );

        angleWithWall = 1.0 - (fabs(angleWithWall) / 2.0f);

        RenderVerticalWallStrip(
            screen,
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
    const ScreenBuffer* const screen,
    const Rect* const viewport,
    const Scene* const scene,
    const int xPosition,
    const int height,
    const float distanceToWall,
    const float angleWithWall,
    CycleProfile* profile)
{
    int wallHeightPixels = 0;

    if (distanceToWall > 0.0f)
    {
        float h = tan(ToRad(scene->player.settings.fov)) * distanceToWall;
        wallHeightPixels = scene->wallHeight / h;
    }
    
    int wallStartY = (height >> 1) - (wallHeightPixels >> 1);
    int wallEndY = (height >> 1) + (wallHeightPixels >> 1);

    for (int i = 0; i < wallStartY; i++)
    {
        DrawPixelViewport(
            screen,
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
        DrawPixelViewport(
            screen,
            viewport,
            &wallColor,
            xPosition,
            i
        );
    }

    for (int i = wallEndY; i < height; i++)
    {
        DrawPixelViewport(
            screen,
            viewport,
            &scene->colors.floorCol,
            xPosition,
            i
        );
    }
}

void RenderWallsTopDown(
    const ScreenBuffer* const screen,
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
            screen,
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
    const ScreenBuffer* const screen,
    const Rect* const viewport,
    const Scene* const scene,
    const Frame2D* const cameraFrame,
    CycleProfile* profile)
{
    Rect rect =
    {
        .x = (int)(scene->player.frame.position.x - 
                  (scene->player.settings.baseSize / 2.0f)),
        .y = (int)(scene->player.frame.position.y - 
                  (scene->player.settings.baseSize / 2.0f)),
        .w = (int)scene->player.settings.baseSize,
        .h = (int)scene->player.settings.baseSize
    };

    RenderCameraSpaceRectangle(
        screen,
        viewport,
        scene,
        cameraFrame,
        &scene->colors.playerCol,
        &rect,
        true
    );
}

void RenderProjectionTopDown(
    const ScreenBuffer* const screen,
    const Rect* const viewport,
    const Scene* const scene,
    const Frame2D* const cameraFrame,
    CycleProfile* profile)
{
    Vector2D worldForward = { .x = 0.0f, .y = -1.0f };
    int widthIncrements = 9;
    float angleInterval = (scene->player.settings.fov * 2.0f) / ((float)(widthIncrements - 1));
    float startAngle = scene->player.frame.theta - scene->player.settings.fov;

    for (int i = 0; i < widthIncrements; i++)
    {
        float theta = startAngle + (i * angleInterval);
        
        Vector2D lookDir =
            FindLookVector(
                &worldForward,
                theta
            );

        lookDir = Vec2DNormalise(lookDir);

        RenderRayTopDown(
            screen,
            viewport,
            scene,
            cameraFrame,
            &lookDir,
            profile
        );
    }
}

void RenderRayTopDown(
    const ScreenBuffer* const screen,
    const Rect* const viewport,
    const Scene* const scene,
    const Frame2D* const cameraFrame,
    const Vector2D* const ray,
    CycleProfile* profile)//
{
    const LineSegment2D* nearestWall = NULL;
    float distanceToWall = DBL_MAX;
    Point2D wallIntersection;
    DLLNode* current = scene->walls.head;

    while (current != NULL)
    {
        const LineSegment2D* const line = (LineSegment2D*)current->data;
        Point2D intersectionPoint;
        float distanceToLine;

        bool doesIntersect =
            DoesRayIntersectLine(
                &scene->player.frame.position,
                ray,
                line,
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
        screen,
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
        .x = (int)(wallIntersection.x - 2.0f),
        .y = (int)(wallIntersection.y - 2.0f),
        .w = 4,
        .h = 4
    };

    RenderCameraSpaceRectangle(
        screen,
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
    const float x, 
    const float y,
    int* xRes, 
    int* yRes)
{
    float t = ToRad(-cameraFrame->theta);
    float s = sin(t);
    float c = cos(t);

    int xr = (int)((x * c) - (y * s) + cameraFrame->position.x);
    int yr = (int)((x * s) + (y * c) + cameraFrame->position.y);

    *xRes = xr;
    *yRes = yr;
}

void RenderCameraSpaceLine(
    const ScreenBuffer* const screen,
    const Rect* const viewport,
    const Scene* const scene,
    const Frame2D* const cameraFrame,
    const Color* const color, 
    const float x1,
    const float y1, 
    const float x2, 
    const float y2)
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
        screen,
        viewport,
        color,
        x1Screen,
        y1Screen,
        x2Screen,
        y2Screen
    );
}

void RenderCameraSpaceRectangle(
    const ScreenBuffer* const screen,
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
            screen,
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
            screen,
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
