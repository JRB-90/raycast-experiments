#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rpifb.h"
#include "rpimailbox.h"

#include <sys/termios.h>

#define PRINT_PROP_DEBUG 1

const char* CON_PATH	= "/dev/tty0";
const char* FB_PATH		= "/dev/fb0";

void TestFBDisplay()
{
	system("setterm -cursor off");

	RpiDisplay display;
	if (InitRpiDisplay(&display, FB_PATH))
	{
		fprintf(stderr, "Startup failed, shutting down...\n");
		system("setterm -cursor on");
		exit(EXIT_FAILURE);
	}

	printf("Original display info\n");
	PrintDisplayInfo(&display);

	if (ChangeDisplay(&display, 640, 480, 16))
	{
		fprintf(stderr, "Change display failed, shutting down...\n");
		system("setterm -cursor on");
		exit(EXIT_FAILURE);
	}

	printf("\nUpdated display info\n");
	PrintDisplayInfo(&display);

	printf("Writing pixels...\n");
	//unsigned int halfScreen = display.screen.size / 2;
	//memset(display.screen.pixels, 0x80, halfScreen);
	//memset(display.screen.pixels + halfScreen, 0xFF, halfScreen);

	for (int i = 0; i < display.screen.w; i++)
	{
		for (int j = 0; j < display.screen.h; j++)
		{
			PlotPixel(&display, i, j, 0xFFFF);
		}
	}

	getchar();

	RevertDisplay(&display);
	printf("\nReverted display info\n");
	PrintDisplayInfo(&display);

	DestroyRpiDisplay(&display);
	system("setterm -cursor on");
	exit(EXIT_SUCCESS);
}

void TestMailbox()
{
	/*RPI_PropertyInit();
	RPI_PropertyAddTag(TAG_ALLOCATE_BUFFER);
	RPI_PropertyAddTag(TAG_SET_PHYSICAL_SIZE, 1280, 1024);
	RPI_PropertyAddTag(TAG_SET_VIRTUAL_SIZE, 1280, 2048);
	RPI_PropertyAddTag(TAG_SET_DEPTH, 32);
	RPI_PropertyAddTag(TAG_GET_PITCH);
	RPI_PropertyAddTag(TAG_GET_PHYSICAL_SIZE);
	RPI_PropertyAddTag(TAG_GET_DEPTH);
	RPI_PropertyProcess();*/

	RPI_PropertyInit();
	RPI_PropertyAddTag(TAG_GET_CLOCK_RATE, TAG_CLOCK_CORE);
	RPI_PropertyProcess();
	rpi_mailbox_property_t* mp = RPI_PropertyGet(TAG_GET_CLOCK_RATE);
	uint32_t core_frequency = mp->data.buffer_32[1];

	printf("Core Freq: %llu \n", core_frequency);

	exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[])
{
	//TestFBDisplay();
	TestMailbox();
}