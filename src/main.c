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

    int halfWidth = CRAY_SCREEN_WIDTH / 2.0;
    int halfHeight = CRAY_SCREEN_HEIGHT / 2.0;

    Scene scene = CreateDefaultScene();
    scene.player.frame.position.x = halfWidth;
    scene.player.frame.position.y = halfHeight;

    Point2D p1 = { .x = halfWidth - 100, .y = halfHeight + 100 };
    Point2D p2 = { .x = halfWidth - 100, .y = halfHeight - 100 };
    Point2D p3 = { .x = halfWidth + 100, .y = halfHeight - 100 };
    Point2D p4 = { .x = halfWidth + 100, .y = halfHeight + 100 };

    LineSegment2D L1 = { .p1 = p1, .p2 = p2 };
    LineSegment2D L2 = { .p1 = p2, .p2 = p3 };
    LineSegment2D L3 = { .p1 = p3, .p2 = p4 };
    LineSegment2D L4 = { .p1 = p4, .p2 = p1 };

    PushDLLNode(&scene.walls, &L1);
    PushDLLNode(&scene.walls, &L2);
    PushDLLNode(&scene.walls, &L3);
    PushDLLNode(&scene.walls, &L4);
    
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
            RenderScene(display, scene);

            previousTicks = currentTicks;
        }
    }

    printf("Closing down...\n");
    CleanupDisplay(display);

	return EXIT_SUCCESS;
}
