#include "display.h"

#include "crayconsts.h"

int CreateDisplay(Display* display, const char* title, int width, int height)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        return CRAY_FAILURE;
    }

	display->width = width;
	display->height = height;
	
	display->window =
        SDL_CreateWindow(
            title,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            width,
            height,
            SDL_WINDOW_SHOWN
        );

    if (display->window == NULL)
    {
        return CRAY_FAILURE;
    }

    display->surface = SDL_GetWindowSurface(display->window);

    if (display->surface == NULL)
    {
        return CRAY_FAILURE;
    }

	return CRAY_SUCCESS;
}

void DestroyDisplay(Display* display)
{
    SDL_FreeSurface(display->surface);
    SDL_DestroyWindow(display->window);
    SDL_Quit();
}
