#ifndef _CR_SDL_DISPLAY_H_
#define _CR_SDL_DISPLAY_H_

#include "crtypes.h"

extern int InitDisplay(
	const ScreenFormat* const desiredFormat, 
	ScreenBuffer* const screen
);
extern int DestroyDisplay(ScreenBuffer* const screen);
extern int RenderDisplay(ScreenBuffer* const screen);

#endif // !_CR_SDL_DISPLAY_H_