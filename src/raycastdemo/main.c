#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "SDL.h"
#include "crconsts.h"
#include "crdisplay.h"
#include "crscene.h"
#include "crinput.h"
#include "crrender.h"
#include "dlist.h"
#include "crmath.h"
#include "raysettings.h"
#include "crtime.h"
#include "crprofile.h"
#include "crdraw.h"

#pragma region Function Defs

// Funtions defs
void PopulateTestTiles(DisplayTile* const tiles);
bool HandleUpdateState(
    InputState* const inputState,
    RaycastSettings* const settings
);
void Update(
    const InputState* const inputState,
    Scene* const scene,
    CycleProfile* const profile
);
void Render(
    const RaycastSettings* const settings,
    const Display* const display,
    const DisplayTile* const tiles,
    const int tileCount,
    Scene* const scene,
    CycleProfile* const profile
);
void PrintSummary(
    const RaycastSettings* const settings,
    const Scene* const scene,
    const CycleProfile* const profile,
    double delta
);

#pragma endregion

// Entry point
int main(int argc, char* argv[])
{
    RaycastSettings settings =
    {
        .printDebugInfo = true,
        .renderMode = Tiled
    };

    printf("Initialising window...\n");

    Display display =
        CreateDisplay(
            "C Raycaster",
            CRAY_SCREEN_WIDTH,
            CRAY_SCREEN_HEIGHT
        );

    printf("Window initialised\n");
    printf("Initialising scene...\n");

    DisplayTile tiles[3];
    PopulateTestTiles(tiles);
    Scene* scene = CreateTestScene(80.0);
    
    printf("Scene initialised\n");
    printf("Starting main loop...\n");
    
    CycleProfile profile = DefaultCycleProfile();
    InputState inputState = DefaultInputState();

    bool isRunning = true;
    double delta;
    double period = 1.0 / (double)CRAY_FPS;
    double targetInterval = period * 1000.0;
    uint64_t currentTicks = GetTicks();
    uint64_t previousTicks = currentTicks;

    while (isRunning)
    {
        currentTicks = GetTicks();
        delta = GetTimeInMS(currentTicks - previousTicks);

        UpdateInputState(&inputState);

        if (HandleUpdateState(&inputState, &settings))
        {
            printf("Received QUIT event from SDL\n");
            isRunning = false;
            continue;
        }

        if (delta > targetInterval)
        {
            Update(
                &inputState, 
                scene, 
                &profile
            );

            Render(
                &settings,
                &display,
                tiles,
                3,
                scene,
                &profile
            );

            PrintSummary(
                &settings,
                scene,
                &profile,
                delta
            );

            ResetProfile(&profile);

            previousTicks = currentTicks;
        }
    }
    
    printf("Closing down...\n");

    CleanupScene(scene);
    CleanupDisplay(&display);

	return EXIT_SUCCESS;
}

#pragma region Function Bodies

// Function bodies
void PopulateTestTiles(DisplayTile* const tiles)
{
    DisplayTile staticSceneTile =
    {
        .tileType = StaticScene,
        .borderColor = CreateColorRGB(255, 255, 0),
        .viewport =
        {
            .x = 40,
            .y = 250,
            .w = 200,
            .h = 200
        }
    };

    DisplayTile staticPlayerTile =
    {
        .tileType = StaticPlayer,
        .borderColor = CreateColorRGB(0, 255, 255),
        .viewport =
        {
            .x = 360,
            .y = 250,
            .w = 200,
            .h = 200
        }
    };

    DisplayTile firstPersonTile =
    {
        .tileType = FirstPerson,
        .borderColor = CreateColorRGB(255, 0, 255),
        .viewport =
        {
            .x = 170,
            .y = 30,
            .w = 300,
            .h = 200
        }
    };

    tiles[0] = staticSceneTile;
    tiles[1] = staticPlayerTile;
    tiles[2] = firstPersonTile;
}
bool HandleUpdateState(
    InputState* const inputState,
    RaycastSettings* const settings)
{
    if (inputState->quit)
    {
        return true;
    }

    if (inputState->toggleDebug)
    {
        settings->printDebugInfo = !settings->printDebugInfo;
    }

    if (inputState->toggleRenderMode)
    {
        if (settings->renderMode == FullStaticScene)
        {
            settings->renderMode = FullFirstPerson;
        }
        else if (settings->renderMode == FullFirstPerson)
        {
            settings->renderMode = Tiled;
        }
        else if (settings->renderMode == Tiled)
        {
            settings->renderMode = FullStaticScene;
        }
    }

    return false;
}

void Update(
    const InputState* const inputState,
    Scene* const scene,
    CycleProfile* const profile)
{
    UpdatePlayerPosition(scene, inputState, profile);
}

void Render(
    const RaycastSettings* const settings,
    const Display* const display,
    const DisplayTile* const tiles,
    const int tileCount,
    Scene* const scene,
    CycleProfile* const profile)
{
    uint64_t renderStartTime = GetTicks();

    if (settings->renderMode == FullStaticScene)
    {
        scene->camera =
            (Frame2D){
                .position =
                {
                    .x = CRAY_SCREEN_WIDTH / 2,
                    .y = CRAY_SCREEN_HEIGHT / 2
                },
                .theta = 0.0
        };
        RenderSceneTopDown(display, scene, profile);
    }
    else if (settings->renderMode == FullFirstPerson)
    {
        scene->camera =
            (Frame2D){
                .position =
                {
                    .x = CRAY_SCREEN_WIDTH / 2,
                    .y = CRAY_SCREEN_HEIGHT / 2
                },
                .theta = 0.0
        };
        RenderSceneFirstPerson(display, scene, profile);
    }
    else if (settings->renderMode == Tiled)
    {
        RenderTiles(display, scene, tiles, tileCount, profile);
    }

    profile->totalRenderTimeMS = GetTimeInMS(GetTicks() - renderStartTime);
}

void PrintSummary(
    const RaycastSettings* const settings,
    const Scene* const scene,
    const CycleProfile* const profile,
    double delta)
{
    uint64_t printStartTime = GetTicks();

    if (settings->printDebugInfo)
    {
        printf("\033[2J");
        printf("\033[H");
        printf(
            "Player: %f, %f, %f\n",
            scene->player.frame.position.x,
            scene->player.frame.position.y,
            scene->player.frame.theta
        );
        printf("Frame time:\t%f ms\n", delta);
        PrintProfileStats(profile);
        printf("Print time:\t%f ms\n", GetTimeInMS(GetTicks() - printStartTime));
    }
}

#pragma endregion
