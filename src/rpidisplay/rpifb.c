#include "rpifb.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

int GetFixedScreenInfo(int frameBufferFD, fb_fix_screeninfo* const screenInfo);
int GetVariableScreenInfo(int frameBufferFD, fb_var_screeninfo* const screenInfo);
int SetVariableScreenInfo(const int frameBufferFD, const fb_var_screeninfo* const screenInfo);
void PrintFixedScreenInfo(const fb_fix_screeninfo* const screenInfo);
void PrintVariableScreenInfo(const fb_var_screeninfo* const screenInfo);
int InitScreen(
	const int frameBufferFD,
	const fb_fix_screeninfo* const fixScreenInfo,
	const fb_var_screeninfo* const varScreenInfo,
	Screen* const screen
);
void DestroyScreen(Screen* const screen);

int InitRpiDisplay(RpiDisplay* const display, const char* const fbPath)
{
	display->frameBufferFD = open(fbPath, O_RDWR);

	if (display->frameBufferFD == -1)
	{
		fprintf(stderr, "Failed to open the framebuffer\n");

		return -1;
	}

	if (GetFixedScreenInfo(display->frameBufferFD, &display->fixedScreenInfo))
	{
		close(display->frameBufferFD);

		return -1;
	}

	if (GetVariableScreenInfo(display->frameBufferFD, &display->originalScreenInfo))
	{
		close(display->frameBufferFD);

		return -1;
	}

	if (GetVariableScreenInfo(display->frameBufferFD, &display->currentScreenInfo))
	{
		close(display->frameBufferFD);

		return -1;
	}

	if (InitScreen(
			display->frameBufferFD,
			&display->fixedScreenInfo,
			&display->currentScreenInfo,
			&display->screen)
		)
	{
		close(display->frameBufferFD);

		return -1;
	}

	return 0;
}

void DestroyRpiDisplay(RpiDisplay* display)
{
	DestroyScreen(&display->screen);
	close(display->frameBufferFD);
}

int ChangeDisplay(RpiDisplay* const display, int xres, int yres, int bitsPP)
{
	fb_var_screeninfo screenInfo = display->currentScreenInfo;
	screenInfo.xres = xres;
	screenInfo.xres_virtual = xres;
	screenInfo.yres = yres;
	screenInfo.yres_virtual = yres;
	screenInfo.bits_per_pixel = bitsPP;

	if (SetVariableScreenInfo(display->frameBufferFD, &screenInfo))
	{
		fprintf(stderr, "Failed to set var screen info\n");

		return -1;
	}

	DestroyScreen(&display->screen);

	if (GetFixedScreenInfo(display->frameBufferFD, &display->fixedScreenInfo))
	{
		close(display->frameBufferFD);

		return -1;
	}

	if (GetVariableScreenInfo(display->frameBufferFD, &display->currentScreenInfo))
	{
		close(display->frameBufferFD);

		return -1;
	}
	
	if (InitScreen(
		display->frameBufferFD,
		&display->fixedScreenInfo,
		&display->currentScreenInfo,
		&display->screen)
	)
	{
		fprintf(stderr, "Failed to recreate screen after display change\n");
		close(display->frameBufferFD);

		return -1;
	}

	return 0;
}

int RevertDisplay(RpiDisplay* const display)
{
	return
		ChangeDisplay(
			display,
			display->originalScreenInfo.xres,
			display->originalScreenInfo.yres,
			display->originalScreenInfo.bits_per_pixel
		);
}

void PrintDisplayInfo(const RpiDisplay* const display)
{
	PrintFixedScreenInfo(&display->fixedScreenInfo);
	PrintVariableScreenInfo(&display->currentScreenInfo);
	printf(
		"Display info: %d size, %d stride\n",
		display->screen.size,
		display->screen.stride
	);
}

void PlotPixel(const RpiDisplay* const display, int x, int y, uint16_t c)
{
	const unsigned int offset = (y * display->screen.stride) + (x * display->screen.bytesPP);
	display->screen.pixels[offset] = (uint8_t)(c & 0xFF);
	display->screen.pixels[offset + 1] = (uint8_t)(c >> 8);
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
		"Var screen info: %dx%d res, %d bpp\n",
		screenInfo->xres,
		screenInfo->yres,
		screenInfo->bits_per_pixel
	);
}

int InitScreen(
	const int frameBufferFD,
	const fb_fix_screeninfo* const fixScreenInfo,
	const fb_var_screeninfo* const varScreenInfo,
	Screen* const screen)
{
	screen->w = varScreenInfo->xres;
	screen->h = varScreenInfo->yres;
	screen->vw = varScreenInfo->xres_virtual;
	screen->vh = varScreenInfo->yres_virtual;
	screen->bitsPP = varScreenInfo->bits_per_pixel;
	screen->bytesPP = varScreenInfo->bits_per_pixel / 8;
	screen->size = varScreenInfo->xres * varScreenInfo->yres * (varScreenInfo->bits_per_pixel / 8);
	screen->stride = varScreenInfo->xres * (varScreenInfo->bits_per_pixel / 8);
	screen->pixels = NULL;

	screen->pixels =
		(uint8_t*)mmap(
			0,
			fixScreenInfo->smem_len,
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

void DestroyScreen(Screen* const screen)
{
	munmap(screen->pixels, screen->size);
}