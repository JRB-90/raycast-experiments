#ifndef _DISPLAY_H
#define _DISPLAY_H

#include "SDL.h"

struct Display {
    SDL_Window* window;
    SDL_Surface* surface;
    int width;
    int height;
};

typedef struct Display Display;

extern int CreateDisplay(Display* display, const char* title, int width, int height);
extern void DestroyDisplay(Display* display);

#endif // !_DISPLAY_H