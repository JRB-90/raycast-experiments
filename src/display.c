#include "display.h"
#include "crayconsts.h"

Color CreateColor()
{
    Color color;
    color.a = 255;
    color.r = 0;
    color.g = 0;
    color.b = 0;

    return color;
}

Color CreateColorRGB(uint8_t r, uint8_t g, uint8_t b)
{
    Color color;
    color.a = 255;
    color.r = r;
    color.g = g;
    color.b = b;

    return color;
}

Color CreateColorARGB(uint8_t a, uint8_t r, uint8_t g, uint8_t b)
{
    Color color;
    color.a = a;
    color.r = r;
    color.g = g;
    color.b = b;

    return color;
}

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

    display->renderer = SDL_CreateRenderer(display->window, -1, 0);

    if (display->renderer == NULL)
    {
        return CRAY_FAILURE;
    }

	return CRAY_SUCCESS;
}

void DestroyDisplay(Display* display)
{
    SDL_DestroyRenderer(display->renderer);
    SDL_DestroyWindow(display->window);
    SDL_Quit();
}
