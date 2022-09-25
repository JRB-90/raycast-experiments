#include "crrpifb_display.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/fb.h>
#include <linux/kd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

const static char* FB_PATH = "/dev/fb0";
const static char* TTY_PATH = "/dev/tty1";
const static char* MB_PATH = "/dev/vcio";

const static unsigned int WIDTH = 640;
const static unsigned int HEIGHT = 480;
const static unsigned int BPP = 32;

typedef struct fb_fix_screeninfo fb_fix_screeninfo;
typedef struct fb_var_screeninfo fb_var_screeninfo;

typedef struct RpiDisplay {
	int fbfd;
	int ttyfd;
	unsigned int screenPixels;
	unsigned int screenSizeBytes;
	unsigned int vscreenPixels;
	unsigned int vscreenSizeBytes;
	fb_fix_screeninfo finfo;
	fb_var_screeninfo vinfo;
	fb_var_screeninfo originalVinfo;
	uint8_t fbReadPage;
	uint8_t fbWritePage;
} RpiDisplay;

RpiDisplay display;

int PanDisplay();
int SwitchPage();
void PrintScreenInfo(
	const fb_fix_screeninfo* const finfo,
	const fb_var_screeninfo* const vinfo
);

int InitDisplay(ScreenBuffer* const screen)
{
	screen->pixels = NULL;
	display.fbfd = NULL;
	display.ttyfd = NULL;
	display.screenSizeBytes = 0;
	display.screenPixels = 0;
	display.fbReadPage = 0;
	display.fbWritePage = 1;

	display.fbfd = open(FB_PATH, O_RDWR);
	if (display.fbfd == -1)
	{
		fprintf(stderr, "Failed to open %s fd\n", FB_PATH);
		return -1;
	}

	if (ioctl(display.fbfd, FBIOGET_FSCREENINFO, &display.finfo))
	{
		fprintf(stderr, "Failed to get fixed screen info\n");
		return -1;
	}

	if (ioctl(display.fbfd, FBIOGET_VSCREENINFO, &display.vinfo))
	{
		fprintf(stderr, "Failed to get variable screen info\n");
		return -1;
	}

	display.originalVinfo = display.vinfo;
	display.screenPixels = display.vinfo.xres * display.vinfo.yres;
	display.screenSizeBytes = display.screenPixels * (display.vinfo.bits_per_pixel / 8);
	display.vscreenPixels = display.vinfo.xres_virtual * display.vinfo.yres_virtual;
	display.vscreenSizeBytes = display.vscreenPixels * (display.vinfo.bits_per_pixel / 8);

	printf("Starting screen info\n");
	PrintScreenInfo(&display.finfo, &display.vinfo);

	display.vinfo.xres = WIDTH;
	display.vinfo.xres_virtual = WIDTH;
	display.vinfo.yres = HEIGHT;
	display.vinfo.yres_virtual = HEIGHT * 2;
	display.vinfo.bits_per_pixel = BPP;
	display.vinfo.yoffset = 0;

	if (ioctl(display.fbfd, FBIOPUT_VSCREENINFO, &display.vinfo))
	{
		fprintf(stderr, "Failed to set variable screen info\n");
		return -1;
	}

	display.screenPixels = display.vinfo.xres * display.vinfo.yres;
	display.screenSizeBytes = display.screenPixels * (display.vinfo.bits_per_pixel / 8);
	display.vscreenPixels = display.vinfo.xres_virtual * display.vinfo.yres_virtual;
	display.vscreenSizeBytes = display.vscreenPixels * (display.vinfo.bits_per_pixel / 8);

	printf("\nUpdated screen info\n");
	PrintScreenInfo(&display.finfo, &display.vinfo);

	display.ttyfd = open(TTY_PATH, O_RDWR);
	if (display.ttyfd == -1)
	{
		fprintf(stderr, "Failed to open %s fd\n", TTY_PATH);
		return -1;
	}

	if (ioctl(display.ttyfd, KDSETMODE, KD_GRAPHICS))
	{
		fprintf(stderr, "Failed to set %s to graphics mode\n", TTY_PATH);
		return -1;
	}

	PanDisplay(screen);

	screen->width = display.vinfo.xres;
	screen->height = display.vinfo.yres;
	screen->bitsPP = display.vinfo.bits_per_pixel;
	screen->bytesPP = display.vinfo.bits_per_pixel / 8;
	screen->size = display.screenSizeBytes;
	screen->stride = display.vinfo.xres * (display.vinfo.bits_per_pixel / 8);

	screen->pixels =
		(uint8_t*)mmap(
			0,
			display.vscreenSizeBytes,
			PROT_READ | PROT_WRITE,
			MAP_SHARED,
			display.fbfd,
			0
		);

	if (screen->pixels == -1)
	{
		fprintf(stderr, "Failed to create the screen, mem map failed\n");

		return -1;
	}

	return 0;
}

int DestroyDisplay(ScreenBuffer* const screen)
{
	if (screen->pixels != NULL)
	{
		munmap(screen->pixels, display.vscreenSizeBytes);
	}

	display.fbReadPage = 1;
	SwitchPage(screen);
	ioctl(display.fbfd, FBIOPUT_VSCREENINFO, &display.originalVinfo);
	ioctl(display.ttyfd, KDSETMODE, KD_TEXT);
	close(display.ttyfd);
	close(display.fbfd);

	return 0;
}

int RenderDisplay(ScreenBuffer* const screen)
{
	return SwitchPage(screen);
}

void PrintScreenInfo(
	const fb_fix_screeninfo* const finfo,
	const fb_var_screeninfo* const vinfo)
{
	printf(
		"Finfo: %s, %d smem len\n",
		finfo->id,
		finfo->smem_len
	);

	printf(
		"Vinfo: %dx%d res, %dx%d vres, %d bpp\n",
		vinfo->xres,
		vinfo->yres,
		vinfo->xres_virtual,
		vinfo->yres_virtual,
		vinfo->bits_per_pixel
	);

	printf("SSize: %d\n", display.screenSizeBytes);
	printf("VSize: %d\n", display.vscreenSizeBytes);
}

int PanDisplay(ScreenBuffer* const screen)
{
	screen->offset = display.fbWritePage * display.screenPixels;
	display.vinfo.yoffset = display.fbReadPage * display.vinfo.yres;
	display.vinfo.activate = FB_ACTIVATE_VBL;

	if (ioctl(display.fbfd, FBIOPAN_DISPLAY, &display.vinfo))
	{
		printf("Failed to pan display\n");
		return -1;
	}

	if (ioctl(display.fbfd, FBIO_WAITFORVSYNC, 0))
	{
		printf("Failed to wait for vsync\n");
		return -1;
	}

	return 0;
}

int SwitchPage(ScreenBuffer* const screen)
{
	if (display.fbReadPage == 0)
	{
		display.fbReadPage = 1;
		display.fbWritePage = 0;
	}
	else
	{
		display.fbReadPage = 0;
		display.fbWritePage = 1;
	}

	return PanDisplay(screen);
}
