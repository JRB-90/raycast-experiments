#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "SDL.h"
#include "crayconsts.h"
#include "display.h"
#include "scene.h"
#include "input.h"
#include "render.h"
#include "dlist/dlist.h"
#include "craymath.h"

// Entry point
int main(int argc, char* argv[])
{
	printf("Initialising window...\n");

    Display display =
        CreateDisplay(
            "C Raycaster",
            CRAY_SCREEN_WIDTH,
            CRAY_SCREEN_HEIGHT
        );

    printf("Window initialised\n");
    printf("Window initialising data...\n");

    Scene scene = CreateDefaultScene();

    double size = 80.0;
    double hsize = size / 2.0;

    Point2D p1 = { .x = -size, .y = -hsize };
    Point2D p2 = { .x = -hsize, .y = hsize };
    Point2D p3 = { .x = size, .y = size };
    Point2D p4 = { .x = size, .y = -hsize };
    Point2D p5 = { .x = hsize, .y = -hsize };
    Point2D p6 = { .x = hsize, .y = -size };
    Point2D p7 = { .x = -hsize, .y = -size };
    Point2D p8 = { .x = -hsize, .y = -hsize };

    LineSegment2D L1 = { .p1 = p1, .p2 = p2 };
    LineSegment2D L2 = { .p1 = p2, .p2 = p3 };
    LineSegment2D L3 = { .p1 = p3, .p2 = p4 };
    LineSegment2D L4 = { .p1 = p4, .p2 = p5 };
    LineSegment2D L5 = { .p1 = p5, .p2 = p6 };
    LineSegment2D L6 = { .p1 = p6, .p2 = p7 };
    LineSegment2D L7 = { .p1 = p7, .p2 = p8 };
    LineSegment2D L8 = { .p1 = p8, .p2 = p1 };

    PushDLLNode(&scene.walls, &L1);
    PushDLLNode(&scene.walls, &L2);
    PushDLLNode(&scene.walls, &L3);
    PushDLLNode(&scene.walls, &L4);
    PushDLLNode(&scene.walls, &L5);
    PushDLLNode(&scene.walls, &L6);
    PushDLLNode(&scene.walls, &L7);
    PushDLLNode(&scene.walls, &L8);

    DisplayTile staticSceneTile =
    {
        .tileType = StaticScene,
        .borderColor = CreateColorRGB(255, 255, 0),
        .position =
        {
            .x = 40.0,
            .y = 250.0,
            .w = 200.0,
            .h = 200.0
        }
    };

    DisplayTile staticPlayerTile =
    {
        .tileType = StaticPlayer,
        .borderColor = CreateColorRGB(0, 255, 255),
        .position =
        {
            .x = 360.0,
            .y = 250.0,
            .w = 200.0,
            .h = 200.0
        }
    };

    DisplayTile firstPersonTile =
    {
        .tileType = FirstPerson,
        .borderColor = CreateColorRGB(255, 0, 255),
        .position =
        {
            .x = 170.0,
            .y = 30.0,
            .w = 300.0,
            .h = 200.0
        }
    };

    DisplayTile tiles[3];
    tiles[0] = staticSceneTile;
    tiles[1] = staticPlayerTile;
    tiles[2] = firstPersonTile;
    
    printf("Data initialised\n");
    printf("Starting main loop...\n");
    
    InputState inputState = DefaultInputState();
    bool isRunning = true;
    double period = 1.0 / (double)CRAY_FPS;
    uint64_t currentTicks = SDL_GetTicks64();
    uint64_t previousTicks = currentTicks;
    uint64_t targetInterval = (uint64_t)(period * 1000.0);
    uint64_t delta;

    while (isRunning)
    {
        currentTicks = SDL_GetTicks64();
        delta = currentTicks - previousTicks;

        UpdateInputState(&inputState);

        if (inputState.quit)
        {
            printf("Received QUIT event from SDL\n");
            isRunning = false;
            continue;
        }

        if (delta > targetInterval)
        {
            printf("Frame delta: %llu\n", delta);

            UpdatePlayerPosition(&scene, inputState);

            printf(
                "Player: %f, %f, %f\n",
                scene.player.frame.position.x,
                scene.player.frame.position.y,
                scene.player.frame.theta
            );

            //scene.camera =
            //(Frame2D) {
            //    .position =
            //    {
            //        .x = CRAY_SCREEN_WIDTH / 2,
            //        .y = CRAY_SCREEN_HEIGHT / 2
            //    },
            //    .theta = 0.0
            //};
            ////RenderSceneTopDown(&display, &scene);
            //RenderSceneFirstPerson(&display, &scene);


            
            //RenderTile(display, scene, tile1);
            //RenderTile(display, scene, tile2);
            //RenderTile(display, scene, tile3);
            
            RenderTiles(&display, &scene, tiles, 3);

            previousTicks = currentTicks;
        }
    }

    printf("Closing down...\n");
    CleanupDisplay(display);

	return EXIT_SUCCESS;
}
