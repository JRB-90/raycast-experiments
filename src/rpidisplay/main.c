#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rpifb.h"

#define FB_PATH "/dev/fb0"

int main(int argc, char* argv[])
{
	int frameBufferFD = OpenFB(FB_PATH);
	PrintFixedScreenInfo(frameBufferFD);
	PrintVariableScreenInfo(frameBufferFD);

	Screen screen = CreateScreen(frameBufferFD);
	memset(screen.pixels, 0xFF, screen.size / 2);
	memset(screen.pixels + screen.size / 2, 0x18, screen.size / 2);

	CloseFB(frameBufferFD);

	exit(EXIT_SUCCESS);
}