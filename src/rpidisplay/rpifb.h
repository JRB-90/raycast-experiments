#ifndef _RPI_FB_H_
#define _RPI_FB_H_

#include <linux/fb.h>
#include <inttypes.h>

typedef struct fb_fix_screeninfo fb_fix_screeninfo;
typedef struct fb_var_screeninfo fb_var_screeninfo;

typedef struct Screen {
	unsigned int w;
	unsigned int h;
	unsigned int vw;
	unsigned int vh;
	unsigned int bitsPP;
	unsigned int bytesPP;
	unsigned int size;
	unsigned int stride;
	uint8_t* pixels;
} Screen;

typedef struct RpiDisplay {
	int frameBufferFD;
	fb_fix_screeninfo fixedScreenInfo;
	fb_var_screeninfo originalScreenInfo;
	fb_var_screeninfo currentScreenInfo;
	Screen screen;
} RpiDisplay;

extern int InitRpiDisplay(RpiDisplay* const display, const char* const fbPath);
extern void DestroyRpiDisplay(RpiDisplay* const display);
extern int ChangeDisplay(RpiDisplay* const display, int xres, int yres, int bitsPP);
extern int RevertDisplay(RpiDisplay* const display);
extern void PrintDisplayInfo(const RpiDisplay* const display);
extern void PlotPixel(const RpiDisplay* const display, int x, int y, uint16_t c);

#endif // !_RPI_FB_H_