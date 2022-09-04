#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <inttypes.h>
#include "SDL.h"
#include "crtypes.h"
#include "crprofile.h"

// Data types
typedef enum TileType {
    StaticScene,
    StaticPlayer,
    FirstPerson
} TileType;

typedef struct DisplayTile {
    Rect viewport;
    TileType tileType;
    Color borderColor;
} DisplayTile;

typedef struct Display {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    uint8_t* pixels;
    int width;
    int height;
} Display;

extern Color CreateColor();
extern Color CreateColorRGB(uint8_t r, uint8_t g, uint8_t b);
extern Color CreateColorARGB(uint8_t a, uint8_t r, uint8_t g, uint8_t b);
extern Display CreateDisplay(const char* const title, int width, int height);
extern void CleanupDisplay(Display* const display);
extern void RenderDisplay(const Display* const display, CycleProfile* const profile);

#endif // !_DISPLAY_H_