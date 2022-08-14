#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "SDL.h"

// Data types
struct Color {
    uint8_t a;
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct Display {
    SDL_Window* window;
    SDL_Renderer* renderer;
    int width;
    int height;
};

// Typedefs
typedef struct Color Color;
typedef struct Display Display;

extern Color CreateColor();
extern Color CreateColorRGB(uint8_t r, uint8_t g, uint8_t b);
extern Color CreateColorARGB(uint8_t a, uint8_t r, uint8_t g, uint8_t b);

extern int CreateDisplay(Display* display, const char* title, int width, int height);
extern void DestroyDisplay(Display* display);

#endif // !_DISPLAY_H_