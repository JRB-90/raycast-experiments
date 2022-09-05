#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "crtime.h"
#include "crtypes.h"
#include "crdraw.h"

const int SCRN_WIDTH			= 640;
const int SCRN_HEIGHT			= 480;
const int WRITE_PIXEL_ITR		= 1000000;
const int DRAW_CLEAR_ITR		= 1000;
const int DRAW_LINE_ITR			= 100000;
const int DRAW_RECT_ITR			= 1000000;

void RunWritePixelTests(const ScreenBuffer* const screen)
{
	printf("==== Write pixel test ====\n");

	Color c = CreateColorRGB(255, 255, 255);

	uint64_t start = GetTicks();

	for (int i = 0; i < WRITE_PIXEL_ITR; i++)
	{
		WritePixel(
			screen,
			&c,
			0,
			0
		);
	}

	double delta = GetTimeInMS(GetTicks() - start);

	printf("%i iterations took %f ms\n\n", WRITE_PIXEL_ITR, delta);
}

void RunWritePixelViewportTests(const ScreenBuffer* const screen)
{
	printf("==== Write pixel viewport test ====\n");

	Color c = CreateColorRGB(255, 255, 255);
	Rect v =
	{
		.x = 0,
		.y = 0,
		.w = SCRN_WIDTH - 1,
		.h = SCRN_HEIGHT - 1
	};

	uint64_t start = GetTicks();

	for (int i = 0; i < WRITE_PIXEL_ITR; i++)
	{
		WritePixelViewport(
			screen,
			&v,
			&c,
			0,
			0
		);
	}

	double delta = GetTimeInMS(GetTicks() - start);

	printf("%i iterations took %f ms\n\n", WRITE_PIXEL_ITR, delta);
}

void RunDrawClearTests(const ScreenBuffer* const screen)
{
	printf("==== Draw clear test ====\n");

	Color c = CreateColorRGB(255, 255, 255);

	uint64_t start = GetTicks();

	for (int i = 0; i < DRAW_CLEAR_ITR; i++)
	{
		DrawClearColor(
			screen,
			&c
		);
	}

	double delta = GetTimeInMS(GetTicks() - start);

	printf("%i iterations took %f ms\n\n", DRAW_CLEAR_ITR, delta);
}

void RunDrawLineTests(const ScreenBuffer* const screen)
{
	printf("==== Draw line test ====\n");

	Color c = CreateColorRGB(255, 255, 255);
	Rect v =
	{
		.x = 0,
		.y = 0,
		.w = SCRN_WIDTH - 1,
		.h = SCRN_HEIGHT - 1
	};

	uint64_t start = GetTicks();

	for (int i = 0; i < DRAW_LINE_ITR; i++)
	{
		DrawLine(screen, &v, &c, 100, 100, 200, 100);
		DrawLine(screen, &v, &c, 100, 100, 200, 200);
		DrawLine(screen, &v, &c, 100, 100, 100, 200);
		DrawLine(screen, &v, &c, 100, 100, 0, 200);
		DrawLine(screen, &v, &c, 100, 100, 0, 100);
		DrawLine(screen, &v, &c, 100, 100, 0, 0);
		DrawLine(screen, &v, &c, 100, 100, 100, 0);
		DrawLine(screen, &v, &c, 100, 100, 200, 0);
	}

	double delta = GetTimeInMS(GetTicks() - start);

	printf("%i iterations took %f ms\n\n", DRAW_LINE_ITR, delta);
}

void RunDrawRectTests(const ScreenBuffer* const screen)
{
	printf("==== Draw rect test ====\n");

	int size = 100;
	Color c = CreateColorRGB(255, 255, 255);
	Rect v =
	{
		.x = 0,
		.y = 0,
		.w = SCRN_WIDTH - 1,
		.h = SCRN_HEIGHT - 1
	};

	uint64_t start = GetTicks();

	for (int i = 0; i < DRAW_LINE_ITR; i++)
	{
		DrawRect(screen, &v, &c, 0, 0, size, size);
	}

	double delta = GetTimeInMS(GetTicks() - start);

	printf("%i iterations took %f ms\n\n", DRAW_LINE_ITR, delta);
}

void RunDrawRectFilledTests(const ScreenBuffer* const screen)
{
	printf("==== Draw rect filled test ====\n");

	int size = 100;
	Color c = CreateColorRGB(255, 255, 255);
	Rect v =
	{
		.x = 0,
		.y = 0,
		.w = SCRN_WIDTH - 1,
		.h = SCRN_HEIGHT - 1
	};

	uint64_t start = GetTicks();

	for (int i = 0; i < DRAW_LINE_ITR; i++)
	{
		DrawRectFilled(screen, &v, &c, 0, 0, size, size);
	}

	double delta = GetTimeInMS(GetTicks() - start);

	printf("%i iterations took %f ms\n\n", DRAW_LINE_ITR, delta);
}

int main(int argc, char* argv[])
{
	ScreenBuffer screen =
	{
		.width = SCRN_WIDTH,
		.height = SCRN_HEIGHT,
		.pixels = malloc(SCRN_WIDTH * SCRN_HEIGHT * 4)
	};

	printf("Begining profile tests..\n\n");

	RunWritePixelTests(&screen);
	RunWritePixelViewportTests(&screen);
	RunDrawClearTests(&screen);
	RunDrawLineTests(&screen);
	RunDrawRectTests(&screen);
	RunDrawRectFilledTests(&screen);

	free(screen.pixels);

	printf("Profile tests complete\n");
	printf("Press any key to finish...\n");
	int res = getchar();

	return EXIT_SUCCESS;
}