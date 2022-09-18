#include "rpifb.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

int OpenFB(const char* const fbPath[])
{
	int frameBufferFD = open(fbPath, O_RDWR);

	if (frameBufferFD == -1)
	{
		printf("Failed to open the framebuffer\n");
		exit(EXIT_FAILURE);
	}

	return frameBufferFD;
}

void CloseFB(int frameBufferFD)
{
	close(frameBufferFD);
}

fb_fix_screeninfo GetFixedScreenInfo(int frameBufferFD)
{
	fb_fix_screeninfo screenInfo;

	if (ioctl(frameBufferFD, FBIOGET_FSCREENINFO, &screenInfo))
	{
		printf("Failed to get the fixed screen info\n");
		exit(EXIT_FAILURE);
	}

	return screenInfo;
}

void PrintFixedScreenInfo(int frameBufferFD)
{
	fb_fix_screeninfo screenInfo = GetFixedScreenInfo(frameBufferFD);

	printf(
		"Fix screen info: %d length\n",
		screenInfo.smem_len
	);
}

struct fb_var_screeninfo GetVariableScreenInfo(int frameBufferFD)
{
	fb_var_screeninfo screenInfo;

	if (ioctl(frameBufferFD, FBIOGET_VSCREENINFO, &screenInfo))
	{
		printf("Failed to get the variable screen info\n");
		exit(EXIT_FAILURE);
	}

	return screenInfo;
}

void SetVariableScreenInfo(
	const int frameBufferFD, 
	const fb_var_screeninfo* const screenInfo)
{
	if (ioctl(frameBufferFD, FBIOPUT_VSCREENINFO, screenInfo))
	{
		printf("Failed to set the variable screen info\n");
		exit(EXIT_FAILURE);
	}
}

void PrintVariableScreenInfo(int frameBufferFD)
{
	fb_var_screeninfo screenInfo = GetVariableScreenInfo(frameBufferFD);

	printf(
		"Var screen info: %dx%d res, %d bpp\n",
		screenInfo.xres,
		screenInfo.yres,
		screenInfo.bits_per_pixel
	);
}

Screen CreateScreen(const int frameBufferFD)
{
	fb_fix_screeninfo fixScreenInfo = GetFixedScreenInfo(frameBufferFD);
	fb_var_screeninfo varScreenInfo = GetVariableScreenInfo(frameBufferFD);

	Screen screen =
	{
		.w = varScreenInfo.xres,
		.h = varScreenInfo.yres,
		.vw = varScreenInfo.xres_virtual,
		.vh = varScreenInfo.yres_virtual,
		.bpp = varScreenInfo.bits_per_pixel,
		.size = fixScreenInfo.smem_len,
		.stride = fixScreenInfo.line_length,
		.pixels = NULL
	};

	screen.pixels =
		(uint8_t*)mmap(
			0,
			fixScreenInfo.smem_len,
			PROT_READ | PROT_WRITE,
			MAP_SHARED,
			frameBufferFD,
			0
		);

	if (screen.pixels == -1)
	{
		printf("Failed to create the screen, mem map failed\n");
		exit(EXIT_FAILURE);
	}

	return screen;
}

void DestroyScreen(Screen* const screen)
{
	munmap(screen->pixels, screen->size);
}
