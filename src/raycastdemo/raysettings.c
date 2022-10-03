#include "raysettings.h"
#include <stdio.h>
#include <stdlib.h>
#include "crconsts.h"

void PrintUsage();
void PrintHelp();

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
            .format = CF_ARGB
        },
        .targetFps = CRAY_DEFAULT_FPS
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
        else if (strcmp(argStr, "-v") == 0 ||
            strcmp(argStr, "--verbose") == 0)
        {
            settings.printDebugInfo = true;
        }
        else if (strcmp(argStr, "-m") == 0 ||
            strcmp(argStr, "--mode") == 0)
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
        else if (strcmp(argStr, "-r") == 0 ||
            strcmp(argStr, "--res") == 0)
        {
            i++;
            if (i >= argc)
            {
                fprintf(stderr, "No res x argument passed\n");
                PrintUsage();
            }

            char* endPtr = NULL;
            unsigned int xres = strtoul(argv[i], endPtr, 10);

            if (errno != 0)
            {
                errno = 0;
                fprintf(stderr, "Invalid res x argument: %s\n", argv[i]);
                PrintUsage();
            }

            i++;
            if (i >= argc)
            {
                fprintf(stderr, "No res y argument passed\n");
                PrintUsage();
            }

            unsigned int yres = strtoul(argv[i], endPtr, 10);

            if (errno != 0)
            {
                errno = 0;
                fprintf(stderr, "Invalid res y argument: %s\n", argv[i]);
                PrintUsage();
            }

            settings.screenFormat.width = xres;
            settings.screenFormat.height = yres;
        }
        else if (strcmp(argStr, "-c") == 0 ||
            strcmp(argStr, "--color") == 0)
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
        else if (strcmp(argStr, "-f") == 0 ||
            strcmp(argStr, "--fps") == 0)
        {
            i++;
            if (i >= argc)
            {
                fprintf(stderr, "No fps argument passed\n");
                PrintUsage();
            }

            char* endPtr = NULL;
            unsigned int fps = strtoul(argv[i], endPtr, 10);
            if (errno != 0)
            {
                errno = 0;
                fprintf(stderr, "Invalid fps argument: %s\n", argv[i]);
                PrintUsage();
            }

            settings.targetFps = fps;
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
    fprintf(stderr, "usage: raycastdemo ");
    fprintf(stderr, "[-v, --verbose] ");
    fprintf(stderr, "[-m, --mode] ");
    fprintf(stderr, "[-r, --res] ");
    fprintf(stderr, "[-c, --color] ");
    fprintf(stderr, "[-f, --fps] ");
    fprintf(stderr, "[-h, --help]\n");
    exit(EXIT_FAILURE);
}

void PrintHelp()
{
    printf("raycastdemo help\n");
    printf("Options:\n");
    printf("\t-v, --verbose: Enable debug printing\n");
    printf("\t-m, --mode:    Set render mode, m -> <Tiled> | <Full>\n");
    printf("\t-r, --res:     Set resolution, r -> <xres> <yres>\n");
    printf("\t-c, --color:   Set the color format, f -> <ARGB> | <RGBA> | <RGB565>\n");
    printf("\t-f, --fps:     Target frame rate\n");
    printf("\t-h, --help:    Display the help text\n");
    exit(EXIT_SUCCESS);
}