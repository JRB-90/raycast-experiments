#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "crtime.h"
#include "crtypes.h"
#include "crdraw.h"
#include "crrender.h"
#include "crscene.h"

const int SCRN_WIDTH			= 640;
const int SCRN_HEIGHT			= 480;

const int WRITE_PIXEL_ITR		= 1000000000;
const int DRAW_CLEAR_ITR		= 1000;
const int DRAW_LINE_ITR			= 100000;
const int DRAW_RECT_ITR			= 1000000;

const int REN_RAY_ITR			= 1000000;
const int REN_PROJ_ITR			= 1000000;
const int REN_PLAY_ITR			= 1000000;
const int REN_WALLS_ITR			= 1000000;
const int REN_VERT_ITR			= 1000000;

void RunWritePixelTests(const ScreenBuffer* const screen)
{
	printf("==== Write pixel test ====\n");

	Color c = CreateColorRGB(255, 255, 255);

	uint64_t start = GetTicks();

	for (int i = 0; i < WRITE_PIXEL_ITR; i++)
	{
		DrawPixel(
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
		DrawPixelViewport(
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

void RunRenderRayTests(const ScreenBuffer* const screen, const Scene* const scene)
{
	printf("==== Render Ray test ====\n");

	Rect v =
	{
		.x = 0,
		.y = 0,
		.w = SCRN_WIDTH - 1,
		.h = SCRN_HEIGHT - 1
	};

	Frame2D c =
	{
		.position =
		{
			.x = SCRN_WIDTH / 2,
			.y = SCRN_HEIGHT / 2
		},
		.theta = 0.0
	};

	Vector2D d = 
	{
		.x = 1.0,
		.y = 0.0
	};

	CycleProfile profile;

	uint64_t start = GetTicks();

	for (int i = 0; i < REN_RAY_ITR; i++)
	{
		RenderRayTopDown(
			screen,
			&v,
			scene,
			&c,
			&d,
			&profile
		);
	}

	double delta = GetTimeInMS(GetTicks() - start);

	printf("%i iterations took %f ms\n\n", REN_RAY_ITR, delta);
}

void RunRenderProjTests(const ScreenBuffer* const screen, const Scene* const scene)
{
	printf("==== Render Projection test ====\n");

	Rect v =
	{
		.x = 0,
		.y = 0,
		.w = SCRN_WIDTH - 1,
		.h = SCRN_HEIGHT - 1
	};

	Frame2D c =
	{
		.position =
		{
			.x = SCRN_WIDTH / 2,
			.y = SCRN_HEIGHT / 2
		},
		.theta = 0.0
	};

	Vector2D d =
	{
		.x = 1.0,
		.y = 0.0
	};

	CycleProfile profile;

	uint64_t start = GetTicks();

	for (int i = 0; i < REN_PROJ_ITR; i++)
	{
		RenderProjectionTopDown(
			screen,
			&v,
			scene,
			&c,
			&profile
		);
	}

	double delta = GetTimeInMS(GetTicks() - start);

	printf("%i iterations took %f ms\n\n", REN_PROJ_ITR, delta);
}

void RunRenderPlayerTests(const ScreenBuffer* const screen, const Scene* const scene)
{
	printf("==== Render Player test ====\n");

	Rect v =
	{
		.x = 0,
		.y = 0,
		.w = SCRN_WIDTH - 1,
		.h = SCRN_HEIGHT - 1
	};

	Frame2D c =
	{
		.position =
		{
			.x = SCRN_WIDTH / 2,
			.y = SCRN_HEIGHT / 2
		},
		.theta = 0.0
	};

	Vector2D d =
	{
		.x = 1.0,
		.y = 0.0
	};

	CycleProfile profile;

	uint64_t start = GetTicks();

	for (int i = 0; i < REN_PLAY_ITR; i++)
	{
		RenderPlayerTopDown(
			screen,
			&v,
			scene,
			&c,
			&profile
		);
	}

	double delta = GetTimeInMS(GetTicks() - start);

	printf("%i iterations took %f ms\n\n", REN_PLAY_ITR, delta);
}

void RunRenderWallsTests(const ScreenBuffer* const screen, const Scene* const scene)
{
	printf("==== Render Walls test ====\n");

	Rect v =
	{
		.x = 0,
		.y = 0,
		.w = SCRN_WIDTH - 1,
		.h = SCRN_HEIGHT - 1
	};

	Frame2D c =
	{
		.position =
		{
			.x = SCRN_WIDTH / 2,
			.y = SCRN_HEIGHT / 2
		},
		.theta = 0.0
	};

	Vector2D d =
	{
		.x = 1.0,
		.y = 0.0
	};

	CycleProfile profile;

	uint64_t start = GetTicks();

	for (int i = 0; i < REN_WALLS_ITR; i++)
	{
		RenderWallsTopDown(
			screen,
			&v,
			scene,
			&c,
			&profile
		);
	}

	double delta = GetTimeInMS(GetTicks() - start);

	printf("%i iterations took %f ms\n\n", REN_WALLS_ITR, delta);
}

void RunRenderVertTests(const ScreenBuffer* const screen, const Scene* const scene)
{
	printf("==== Render Vert test ====\n");

	Rect v =
	{
		.x = 0,
		.y = 0,
		.w = SCRN_WIDTH - 1,
		.h = SCRN_HEIGHT - 1
	};

	Frame2D c =
	{
		.position =
		{
			.x = SCRN_WIDTH / 2,
			.y = SCRN_HEIGHT / 2
		},
		.theta = 0.0
	};

	Vector2D d =
	{
		.x = 1.0,
		.y = 0.0
	};

	CycleProfile profile;

	uint64_t start = GetTicks();

	for (int i = 0; i < REN_VERT_ITR; i++)
	{
		RenderVerticalWallStrip(
			screen,
			&v,
			scene,
			10,
			200,
			100.0,
			0.5,
			&profile
		);
	}

	double delta = GetTimeInMS(GetTicks() - start);

	printf("%i iterations took %f ms\n\n", REN_VERT_ITR, delta);
}

int main(int argc, char* argv[])
{
	ScreenBuffer screen =
	{
		.width = SCRN_WIDTH,
		.height = SCRN_HEIGHT,
		.pixels = malloc(SCRN_WIDTH * SCRN_HEIGHT * 4)
	};

	Scene* scene = CreateTestScene(80.0);

	printf("Begining profile tests..\n\n");

	//RunWritePixelTests(&screen);
	//RunWritePixelViewportTests(&screen);
	//RunDrawClearTests(&screen);
	//RunDrawLineTests(&screen);
	//RunDrawRectTests(&screen);
	//RunDrawRectFilledTests(&screen);

	//RunRenderRayTests(&screen, scene);
	//RunRenderProjTests(&screen, scene);
	//RunRenderPlayerTests(&screen, scene);
	//RunRenderWallsTests(&screen, scene);
	RunRenderVertTests(&screen, scene);

	CleanupScene(scene);
	free(screen.pixels);

	printf("Profile tests complete\n");
	printf("Press any key to finish...\n");
	int res = getchar();

	return EXIT_SUCCESS;
}