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
	unsigned int bpp;
	unsigned int size;
	unsigned int stride;
	uint8_t* pixels;
} Screen;

extern int OpenFB(const char* const fbPath[]);
extern void CloseFB(int frameBufferFD);
extern fb_fix_screeninfo GetFixedScreenInfo(int frameBufferFD);
extern void PrintFixedScreenInfo(int frameBufferFD);
extern fb_var_screeninfo GetVariableScreenInfo(int frameBufferFD);
extern void SetVariableScreenInfo(
	const int frameBufferFD, 
	const fb_var_screeninfo* const screenInfo
);
extern void PrintVariableScreenInfo(int frameBufferFD);
extern Screen CreateScreen(const int frameBufferFD);
extern void DestroyScreen(Screen* const screen);

#endif // !_RPI_FB_H_