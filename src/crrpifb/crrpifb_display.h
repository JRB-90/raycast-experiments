#ifndef _CR_RPI_DISPLAY_H_
#define _CR_RPI_DISPLAY_H_

#include <linux/fb.h>
#include <inttypes.h>
#include "crtypes.h"
#include "crprofile.h"

typedef struct fb_fix_screeninfo fb_fix_screeninfo;
typedef struct fb_var_screeninfo fb_var_screeninfo;

typedef struct RpiDisplay {
	int frameBufferFD;
	fb_fix_screeninfo fixedScreenInfo;
	fb_var_screeninfo originalScreenInfo;
	fb_var_screeninfo currentScreenInfo;
} RpiDisplay;

extern int InitDisplay(ScreenBuffer* const screen);
extern int DestroyDisplay(ScreenBuffer* const screen);
extern int RenderDisplay(ScreenBuffer* const screen, CycleProfile* const profile);

#endif // !_CR_RPI_DISPLAY_H_