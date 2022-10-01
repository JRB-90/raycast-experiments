#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdbool.h>
#include <string.h>
#include "raysettings.h"
#include "crconsts.h"
#include "crtypes.h"
#include "crscene.h"
#include "crrender.h"
#include "crtime.h"
#include "crprofile.h"

#ifdef CRSDL
#include "crsdl2_input.h"
#include "crsdl2_display.h"
#endif // CRSDL

#ifdef CRRPIFB
#include "crrpidfb_input.h"
#include "crrpidfb_display.h"
#endif // CRSDL

#pragma region Function Defs

// Funtions defs
void SignalHandler(int signum);
void Cleanup(int status);
RaycastSettings ParseCommandLine(
    int argc, 
    char* argv[]
);
void PrintUsage();
void PrintHelp();
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
    const DisplayTile* const tiles,
    const ScreenBuffer* const screen,
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

// Globals
Scene* scene;
ScreenBuffer screen;

// Entry point
int main(int argc, char* argv[])
{
    scene = NULL;
    screen = DefaultScreen();
    CycleProfile profile = DefaultCycleProfile();
    InputState inputState = DefaultInputState();
    RaycastSettings settings = ParseCommandLine(argc, argv);

    signal(SIGINT, SignalHandler);

    printf("Initialising input...\n");

    if (InitInputDevice())
    {
        fprintf(stderr, "Window initialisation failed, shutting down...\n");
        Cleanup(EXIT_FAILURE);
    }

    printf("Input initialised\n");
    printf("Initialising window...\n");

    if (InitDisplay(&settings.screenFormat, &screen))
    {
        fprintf(stderr, "Window initialisation failed, shutting down...\n");
        Cleanup(EXIT_FAILURE);
    }

    printf("Window initialised\n");
    printf("Initialising scene...\n");

    DisplayTile tiles[3];
    PopulateTestTiles(tiles);
    scene = CreateTestScene(80.0);
    
    printf("Scene initialised\n");
    printf("Starting main loop...\n");
    
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
            printf("Received QUIT event from input\n");
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
                tiles,
                &screen,
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

    Cleanup(EXIT_SUCCESS);
}

#pragma region Function Bodies

// Function bodies
void SignalHandler(int signum)
{
    if (signum == SIGTERM ||
        signum == SIGABRT ||
        signum == SIGINT)
    {
        Cleanup(EXIT_SUCCESS);
    }
}

void Cleanup(int status)
{
    if (scene != NULL)
    {
        CleanupScene(scene);
    }
    DestroyInputDevice();
    DestroyDisplay(&screen);
    exit(status);
}

RaycastSettings ParseCommandLine(
    int argc, 
    char* argv[])
{
    RaycastSettings settings =
    {
        .printDebugInfo = false,
        .renderMode = Tiled,
        .screenFormat = (ScreenFormat)
        {
            .width = 640,
            .height = 480,
            .format = CF_RGB565
        }
    };

    int i = 1;
    while (i < argc)
    {
        char* argStr = argv[i];
        if (strcmp(argStr, "-h") == 0 ||
            strcmp(argStr, "--help") == 0)
        {
            PrintHelp();
        }
        else if (strcmp(argStr, "-v") == 0)
        {
            settings.printDebugInfo = true;
        }
        else if (strcmp(argStr, "-mode") == 0)
        {
            i++;

            if (i >= argc)
            {
                fprintf(stderr, "No mode argument passed\n");
                PrintUsage();
            }

            if (strcmp(argv[i], "Tiled") == 0)
            {
                settings.renderMode = Tiled;
            }
            else if (strcmp(argv[i], "Full") == 0)
            {
                settings.renderMode = FullFirstPerson;
            }
            else
            {
                fprintf(stderr, "Invalid mode argument: %s\n", argv[i]);
                PrintUsage();
            }
        }
        else if (strcmp(argStr, "-res") == 0)
        {
            i++;
            if (i >= argc)
            {
                fprintf(stderr, "No res x argument passed\n");
                PrintUsage();
            }

            int xres = atoi(argv[i]);
            if (xres == 0)
            {
                fprintf(stderr, "Invalid res x argument: %s\n", argv[i]);
                PrintUsage();
            }

            i++;
            if (i >= argc)
            {
                fprintf(stderr, "No res y argument passed\n");
                PrintUsage();
            }

            int yres = atoi(argv[i]);
            if (yres == 0)
            {
                fprintf(stderr, "Invalid res y argument: %s\n", argv[i]);
                PrintUsage();
            }

            settings.screenFormat.width = xres;
            settings.screenFormat.height = yres;
        }
        else if (strcmp(argStr, "-format") == 0)
        {
            i++;
            if (i >= argc)
            {
                fprintf(stderr, "No format argument passed\n");
                PrintUsage();
            }

            if (strcmp(argv[i], "ARGB") == 0)
            {
                settings.screenFormat.format = CF_ARGB;
            }
            else if (strcmp(argv[i], "RGBA") == 0)
            {
                settings.screenFormat.format = CF_RGBA;
            }
            else if (strcmp(argv[i], "RGB565") == 0)
            {
                settings.screenFormat.format = CF_RGB565;
            }
            else
            {
                fprintf(stderr, "Invalid format argument: %s\n", argv[i]);
                PrintUsage();
            }
        }
        else
        {
            fprintf(stderr, "Invalid argument: %s\n", argStr);
            PrintUsage();
        }

        i++;
    }

    return settings;
}

void PrintUsage()
{
    fprintf(stderr, "usage: raycastdemo [-v] [-mode m] [-res r] [-format f] [-h]\n");
    exit(EXIT_FAILURE);
}

void PrintHelp()
{
    printf("raycastdemo help\n");
    printf("Options:\n");
    printf("\t-v:        Enable debug printing\n");
    printf("\t-mode m:   Set render mode, m -> <Tiled> | <Full>\n");
    printf("\t-res r:    Set resolution, r -> <xres> <yres>\n");
    printf("\t-format f: Set the color format, f -> <ARGB> | <RGBA> | <RGB565>\n");
    printf("\t-h:        Display the help text\n");
    exit(EXIT_SUCCESS);
}

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
    const DisplayTile* const tiles,
    const ScreenBuffer* const screen,
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
        RenderSceneTopDown(screen, scene, profile);
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
        RenderSceneFirstPerson(screen, scene, profile);
    }
    else if (settings->renderMode == Tiled)
    {
        RenderTiles(screen, scene, tiles, tileCount, profile);
    }

    uint64_t presentStartTime = GetTicks();
    RenderDisplay(screen);
    profile->renderPresentTimeMS = GetTimeInMS(GetTicks() - presentStartTime);
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
