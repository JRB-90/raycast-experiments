#include "crrpifb_display.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

RpiDisplay display;

static const char* FB_PATH = "/dev/fb0";

int GetFixedScreenInfo(int frameBufferFD, fb_fix_screeninfo* const screenInfo);
int GetVariableScreenInfo(int frameBufferFD, fb_var_screeninfo* const screenInfo);
int SetVariableScreenInfo(const int frameBufferFD, const fb_var_screeninfo* const screenInfo);
void PrintFixedScreenInfo(const fb_fix_screeninfo* const screenInfo);
void PrintVariableScreenInfo(const fb_var_screeninfo* const screenInfo);
int InitScreen(
	const int frameBufferFD,
	const fb_fix_screeninfo* const fixScreenInfo,
	const fb_var_screeninfo* const varScreenInfo,
	ScreenBuffer* const screen
);
void DestroyScreen(ScreenBuffer* const screen);
void PlotPixel(
	const RpiDisplay* const display,
	const ScreenBuffer* const screen,
	int x,
	int y,
	uint16_t c
);

int InitDisplay(ScreenBuffer* const screen)
{
	display.frameBufferFD = open(FB_PATH, O_RDWR);

	if (display.frameBufferFD == -1)
	{
		fprintf(stderr, "Failed to open the framebuffer\n");

		return -1;
	}

	if (GetFixedScreenInfo(display.frameBufferFD, &display.fixedScreenInfo))
	{
		close(display.frameBufferFD);

		return -1;
	}

	if (GetVariableScreenInfo(display.frameBufferFD, &display.originalScreenInfo))
	{
		close(display.frameBufferFD);

		return -1;
	}

	if (GetVariableScreenInfo(display.frameBufferFD, &display.currentScreenInfo))
	{
		close(display.frameBufferFD);

		return -1;
	}

	PrintFixedScreenInfo(&display.fixedScreenInfo);
	PrintVariableScreenInfo(&display.currentScreenInfo);

	display.currentScreenInfo.xres = 640;
	display.currentScreenInfo.xres_virtual = display.currentScreenInfo.xres;
	display.currentScreenInfo.yres = 480;
	display.currentScreenInfo.yres_virtual = display.currentScreenInfo.yres;
	display.currentScreenInfo.bits_per_pixel = 16;
	//display.currentScreenInfo.yoffset = 0;

	if (SetVariableScreenInfo(display.frameBufferFD, &display.currentScreenInfo))
	{
		close(display.frameBufferFD);

		return -1;
	}

	if (InitScreen(
		display.frameBufferFD,
		&display.fixedScreenInfo,
		&display.currentScreenInfo,
		screen)
	)
	{
		SetVariableScreenInfo(display.frameBufferFD, &display.originalScreenInfo);
		close(display.frameBufferFD);

		return -1;
	}

	return 0;
}

int DestroyDisplay(ScreenBuffer* const screen)
{
	DestroyScreen(screen);
	SetVariableScreenInfo(display.frameBufferFD, &display.currentScreenInfo);
	close(display.frameBufferFD);

	return 0;
}

int RenderDisplay(ScreenBuffer* const screen, CycleProfile* const profile)
{
	return 0;
}

int GetFixedScreenInfo(int frameBufferFD, fb_fix_screeninfo* const screenInfo)
{
	if (ioctl(frameBufferFD, FBIOGET_FSCREENINFO, screenInfo))
	{
		fprintf(stderr, "Failed to get the fixed screen info\n");

		return -1;
	}

	return 0;
}

int GetVariableScreenInfo(int frameBufferFD, fb_var_screeninfo* const screenInfo)
{
	if (ioctl(frameBufferFD, FBIOGET_VSCREENINFO, screenInfo))
	{
		fprintf(stderr, "Failed to get the variable screen info\n");

		return -1;
	}

	return 0;
}

int SetVariableScreenInfo(
	const int frameBufferFD,
	const fb_var_screeninfo* const screenInfo)
{
	if (ioctl(frameBufferFD, FBIOPUT_VSCREENINFO, screenInfo))
	{
		fprintf(stderr, "Failed to set the variable screen info\n");

		return -1;
	}

	return 0;
}

void PrintFixedScreenInfo(const fb_fix_screeninfo* const screenInfo)
{
	printf(
		"Fix screen info: %d mem len, %d line len\n",
		screenInfo->smem_len,
		screenInfo->line_length
	);
}

void PrintVariableScreenInfo(const fb_var_screeninfo* const screenInfo)
{
	printf(
		"Var screen info: %dx%d rres, %dx%d vres, %d bpp\n",
		screenInfo->xres,
		screenInfo->yres,
		screenInfo->xres_virtual,
		screenInfo->yres_virtual,
		screenInfo->bits_per_pixel
	);
}

int InitScreen(
	const int frameBufferFD,
	const fb_fix_screeninfo* const fixScreenInfo,
	const fb_var_screeninfo* const varScreenInfo,
	ScreenBuffer* const screen)
{
	screen->width = varScreenInfo->xres;
	screen->height = varScreenInfo->yres;
	screen->bitsPP = varScreenInfo->bits_per_pixel;
	screen->bytesPP = varScreenInfo->bits_per_pixel / 8;
	screen->size = varScreenInfo->xres_virtual * varScreenInfo->yres_virtual * (varScreenInfo->bits_per_pixel / 8);
	screen->stride = varScreenInfo->xres * (varScreenInfo->bits_per_pixel / 8);
	screen->pixels = NULL;

	screen->pixels =
		(uint8_t*)mmap(
			0,
			screen->size,
			PROT_READ | PROT_WRITE,
			MAP_SHARED,
			frameBufferFD,
			0
		);

	if (screen->pixels == -1)
	{
		fprintf(stderr, "Failed to create the screen, mem map failed\n");

		return -1;
	}

	return 0;
}

void DestroyScreen(ScreenBuffer* const screen)
{
	munmap(screen->pixels, screen->size);
}

void PlotPixel(
	const RpiDisplay* const display,
	const ScreenBuffer* const screen,
	int x, 
	int y,
	uint16_t c)
{
	const unsigned int offset = (y * screen->stride) + (x * screen->bytesPP);
	screen->pixels[offset] = (uint8_t)(c & 0xFF);
	screen->pixels[offset + 1] = (uint8_t)(c >> 8);
}