#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <inttypes.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <linux/kd.h>
#include <linux/input.h>
#include <linux/input-event-codes.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <termios.h>

#define MAJOR_NUM 100
#define IOCTL_MBOX_PROPERTY _IOWR(MAJOR_NUM, 0, char *)

const static char* FB_PATH			= "/dev/fb0";
const static char* TTY_PATH			= "/dev/tty1";
const static char* MB_PATH			= "/dev/vcio";
const static char* EVNT_PATH		= "/dev/input/event0";

const static unsigned int WIDTH		= 640;
const static unsigned int HEIGHT	= 480;
const static unsigned int BPP		= 32;

typedef struct fb_fix_screeninfo fb_fix_screeninfo;
typedef struct fb_var_screeninfo fb_var_screeninfo;
typedef struct input_event input_event;

int fbfd;
int ttyfd;
int mbfd;
int evfd;
uint32_t* fbp;
unsigned int screenPixels;
unsigned int screenSizeBytes;
unsigned int vscreenPixels;
unsigned int vscreenSizeBytes;
fb_fix_screeninfo finfo;
fb_var_screeninfo vinfo;
fb_var_screeninfo originalVinfo;
uint8_t fbReadPage;
uint8_t fbWritePage;
input_event events[64];

void SignalHandler(int signum)
{
	if (signum == SIGTERM ||
		signum == SIGABRT ||
		signum == SIGINT)
	{
		Cleanup(EXIT_SUCCESS);
	}
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

	printf("SSize: %d\n", screenSizeBytes);
	printf("VSize: %d\n", vscreenSizeBytes);
}

void GetMBoxProperty(void* buf)
{
	if (ioctl(mbfd, IOCTL_MBOX_PROPERTY, buf))
	{
		fprintf(stderr, "Failed to get mailbox property");
		Cleanup(EXIT_FAILURE);
	}
}

unsigned int SetFBOffset(unsigned int x, unsigned int y)
{
	int i = 0;
	unsigned int p[32];

	p[i++] = 0;				// size
	p[i++] = 0x00000000;	// process request
	p[i++] = 0x00048009;	// get physical (display) width/height
	p[i++] = 0x00000008;	// buffer size
	p[i++] = 0x00000000;	// request size
	p[i++] = x;				// value buffer
	p[i++] = y;				// value buffer 2
	p[i++] = 0x00000000;	// end tag
	p[0] = i * sizeof * p;	// actual size

	GetMBoxProperty(p);

	return p[1];
}

void PanDisplay()
{
	vinfo.yoffset = fbReadPage * vinfo.yres;
	vinfo.activate = FB_ACTIVATE_VBL;

	if (ioctl(fbfd, FBIOPAN_DISPLAY, &vinfo))
	{
		printf("Failed to pan display\n");
		Cleanup(EXIT_FAILURE);
	}

	if (ioctl(fbfd, FBIO_WAITFORVSYNC, 0))
	{
		printf("Failed to wait for vsync\n");
		Cleanup(EXIT_FAILURE);
	}
}

void SwitchPage()
{
	if (fbReadPage == 0)
	{
		fbReadPage = 1;
		fbWritePage = 0;
	}
	else
	{
		fbReadPage = 0;
		fbWritePage = 1;
	}

	PanDisplay();
}

void Init()
{
	fbfd = open(FB_PATH, O_RDWR);
	if (fbfd == -1)
	{
		fprintf(stderr, "Failed to open %s fd\n", FB_PATH);
		Cleanup(EXIT_FAILURE);
	}

	if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo))
	{
		fprintf(stderr, "Failed to get fixed screen info\n");
		Cleanup(EXIT_FAILURE);
	}

	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo))
	{
		fprintf(stderr, "Failed to get variable screen info\n");
		Cleanup(EXIT_FAILURE);
	}

	originalVinfo = vinfo;
	screenPixels = vinfo.xres * vinfo.yres;
	screenSizeBytes = screenPixels * (vinfo.bits_per_pixel / 8);
	vscreenPixels = vinfo.xres_virtual * vinfo.yres_virtual;
	vscreenSizeBytes = vscreenPixels * (vinfo.bits_per_pixel / 8);

	printf("Starting screen info\n");
	PrintScreenInfo(&finfo, &vinfo);

	vinfo.xres = WIDTH;
	vinfo.xres_virtual = WIDTH;
	vinfo.yres = HEIGHT;
	vinfo.yres_virtual = HEIGHT * 2;
	vinfo.bits_per_pixel = BPP;
	vinfo.yoffset = 0;

	if (ioctl(fbfd, FBIOPUT_VSCREENINFO, &vinfo))
	{
		fprintf(stderr, "Failed to set variable screen info\n");
		Cleanup(EXIT_FAILURE);
	}

	screenPixels = vinfo.xres * vinfo.yres;
	screenSizeBytes = screenPixels * (vinfo.bits_per_pixel / 8);
	vscreenPixels = vinfo.xres_virtual * vinfo.yres_virtual;
	vscreenSizeBytes = vscreenPixels * (vinfo.bits_per_pixel / 8);

	printf("\nUpdated screen info\n");
	PrintScreenInfo(&finfo, &vinfo);

	ttyfd = open(TTY_PATH, O_RDWR);
	if (ttyfd == -1)
	{
		fprintf(stderr, "Failed to open %s fd\n", TTY_PATH);
		Cleanup(EXIT_FAILURE);
	}

	if (ioctl(ttyfd, KDSETMODE, KD_GRAPHICS))
	{
		fprintf(stderr, "Failed to set %s to graphics mode\n", TTY_PATH);
		Cleanup(EXIT_FAILURE);
	}

	mbfd = open(MB_PATH, 0);
	if (mbfd == -1)
	{
		fprintf(stderr, "Failed to open mailbox fd\n");
		Cleanup(EXIT_FAILURE);
	}

	PanDisplay();

	fbp =
		(uint32_t*)mmap(
			0,
			vscreenSizeBytes,
			PROT_READ | PROT_WRITE,
			MAP_SHARED,
			fbfd,
			0
		);

	if (fbp == -1)
	{
		fprintf(stderr, "Failed to set %s to graphics mode\n", TTY_PATH);
		Cleanup(EXIT_FAILURE);
	}
}

void Cleanup(int status)
{
	if (fbp != NULL)
	{
		munmap(fbp, screenSizeBytes);
	}

	SetFBOffset(0, 0);
	ioctl(fbfd, FBIOPUT_VSCREENINFO, &originalVinfo);
	ioctl(ttyfd, KDSETMODE, KD_TEXT);
	close(mbfd);
	close(ttyfd);
	close(fbfd);
	exit(status);
}

void WritePixel(unsigned int x, unsigned int y, uint32_t color)
{
	unsigned int offset = fbWritePage * screenPixels;
	unsigned int pixOffset = offset + x + (y * vinfo.xres);
	fbp[pixOffset] = color;
}

void ClearScreen(uint32_t color)
{
	unsigned int offset = fbWritePage * screenPixels;

	for (unsigned int i = offset; i < screenPixels + offset; i++)
	{
		fbp[i] = color;
	}
}

void FillRect(
	unsigned int x, 
	unsigned int y, 
	unsigned int w, 
	unsigned int h, 
	uint32_t color)
{
	for (unsigned int i = x; i < x + w; i++)
	{
		for (unsigned int j = y; j < y + h; j++)
		{
			WritePixel(i, j, color);
		}
	}
}

int main(int argc, char* argv[])
{
	/*fbfd = NULL;
	ttyfd = NULL;
	mbfd = NULL;
	fbp = NULL;
	screenSizeBytes = 0;
	screenPixels = 0;
	fbReadPage = 0;
	fbWritePage = 1;

	signal(SIGINT, SignalHandler);

	Init();

	unsigned int xPos = 0;

	while (1)
	{
		for (unsigned int xPos = 50; xPos < 200; xPos++)
		{
			ClearScreen(0xFFFF0000);
			FillRect(xPos, 50, 100, 100, 0xFF0000FF);
			SwitchPage();
		}

		for (unsigned int xPos = 200; xPos > 50; xPos--)
		{
			ClearScreen(0xFFFF0000);
			FillRect(xPos, 50, 100, 100, 0xFF0000FF);
			SwitchPage();
		}
	}

	getchar();

	Cleanup(EXIT_SUCCESS);*/

	

	struct termios old, new;
	tcgetattr(STDIN_FILENO, &old);
	new = old;
	new.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &new);

	evfd = open(EVNT_PATH, O_RDONLY);
	if (evfd == -1)
	{
		fprintf(stderr, "Failed to open event fd\n");
		exit(EXIT_FAILURE);
	}

	int version;
	if (ioctl(evfd, EVIOCGVERSION, &version))
	{
		fprintf(stderr, "Failed to get input version\n");
		close(evfd);
		exit(EXIT_FAILURE);
	}

	printf(
		"Input driver version: %d.%d.%d\n",
		version >> 16,
		(version >> 8) & 0xff,
		version & 0xff
	);

	unsigned short id[4];
	if (ioctl(evfd, EVIOCGID, id))
	{
		fprintf(stderr, "Failed to get input id\n");
		close(evfd);
		exit(EXIT_FAILURE);
	}

	printf(
		"Input device ID: bus 0x%x vendor 0x%x product 0x%x version 0x%x\n",
		id[ID_BUS],
		id[ID_VENDOR],
		id[ID_PRODUCT],
		id[ID_VERSION]
	);

	int isRunning = 1;
	while (isRunning)
	{
		int bytesRead = read(evfd, events, sizeof(input_event) * 64);
		if (bytesRead < (int)sizeof(input_event))
		{
			fprintf(stderr, "Failed to read keyboard\n");
			tcsetattr(STDIN_FILENO, TCSANOW, &old);
			tcflush(STDIN_FILENO, TCIOFLUSH);
			close(evfd);
			exit(EXIT_FAILURE);
		}
		
		for (int i = 0; i < (bytesRead / sizeof(input_event)); i++)
		{
			if (events[i].type == EV_KEY)
			{
				if (events[i].value == 1)
				{
					printf("KEY DOWN %u\n", events[i].code);

					if (events[i].code == KEY_Q)
					{
						isRunning = 0;
						break;
					}
				}
				else if (events[i].value == 0)
				{
					printf("KEY UP %u\n", events[i].code);
				}
			}
		}
	}

	tcsetattr(STDIN_FILENO, TCSANOW, &old);
	tcflush(STDIN_FILENO, TCIOFLUSH);
	close(evfd);
	exit(EXIT_SUCCESS);
}