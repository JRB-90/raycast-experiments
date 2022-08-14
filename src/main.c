#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "SDL.h"
#include "crayconsts.h"
#include "display.h"
#include "scene.h"
#include "input.h"
#include "render.h"

// Main function
int main(int argc, char* argv[])
{
    Display display;
    Scene scene;
    InputState inputState;

	printf("Initialising window...\n");

    if (CreateDisplay(&display, "C Raycaster", CRAY_SCREEN_WIDTH, CRAY_SCREEN_HEIGHT))
    {
        HandleError("Failed to initialise window...");
    }

    printf("Window initialised\n");
    printf("Window initialising data...\n");

    InitDefaultScene(&scene);
    inputState = CreateInputState();

    printf("Data initialised\n");
    printf("Starting main loop...\n");
    
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

            // Update scene
            
            RenderScene(&display, &scene);

            previousTicks = currentTicks;
        }
    }

    printf("Closing down...\n");
    DestroyDisplay(&display);

	return EXIT_SUCCESS;
}
