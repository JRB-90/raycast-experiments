#ifndef _CR_DISPLAY_H_
#define _CR_DISPLAY_H_

#include <inttypes.h>
#include "SDL.h"
#include "crtypes.h"
#include "crprofile.h"

// Data types
typedef struct SdlDisplay {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
} SdlDisplay;

extern int InitDisplay(ScreenBuffer* const screen);
extern int DestroyDisplay(ScreenBuffer* const screen);
extern int RenderDisplay(ScreenBuffer* const screen, CycleProfile* const profile);

#endif // !_CR_DISPLAY_H_