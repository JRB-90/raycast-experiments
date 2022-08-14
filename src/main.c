#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "SDL.h"
#include "display.h"
#include "crayconsts.h"

// Functions bodies
void HandleError(const char* errorMessage)
{
    fprintf(stderr, "\033[0;31m");
    fprintf(stderr, "Error: %s\n", errorMessage);
    int c = getchar();
    exit(EXIT_FAILURE);
}

// Main function
int main(int argc, char* argv[])
{
    Display display;

	printf("Initialising window...\n");

    if (CreateDisplay(&display, "C Raycaster", CRAY_SCREEN_WIDTH, CRAY_SCREEN_HEIGHT))
    {
        HandleError("Failed to initialise window...");
    }

    printf("Window initialised\n");
    printf("Starting main loop...\n");

    bool isRunning = true;
    while (isRunning)
    {
        SDL_Event e;

        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                printf("Received QUIT event from SDL\n");
                isRunning = false;
                break;
            }
        }
    }

    printf("Closing down...\n");
    DestroyDisplay(&display);

	return EXIT_SUCCESS;
}