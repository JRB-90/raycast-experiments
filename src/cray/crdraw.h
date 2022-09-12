#ifndef _CR_DRAW_H_
#define _CR_DRAW_H_

#include "crtypes.h"

extern void DrawClearColor(
	const ScreenBuffer* const screen,
	const Color* const color
);
extern void DrawLine(
	const ScreenBuffer* const screen,
	const Rect* const viewport,
	const Color* const color,
	int x1,
	int y1,
	int x2, 
	int y2
);
extern void DrawRect(
	const ScreenBuffer* const screen,
	const Rect* const viewport,
	const Color* const color,
	int x, 
	int y, 
	int w, 
	int h
);
extern void DrawRectFilled(
	const ScreenBuffer* const screen,
	const Rect* const viewport,
	const Color* const color,
	int x,
	int y,
	int w,
	int h
);

// Static inline defs
static inline void DrawPixel(
	const ScreenBuffer* const screen,
	const Color* const color,
	int x,
	int y)
{
	const int offset = (screen->width * y * 4) + x * 4;
	screen->pixels[offset + 0] = color->a;
	screen->pixels[offset + 1] = color->b;
	screen->pixels[offset + 2] = color->g;
	screen->pixels[offset + 3] = color->r;
}

static inline void DrawPixelViewport(
	const ScreenBuffer* const screen,
	const Rect* const viewport,
	const Color* const color,
	int x,
	int y)
{
	if (x >= 0 &&
		x < viewport->w &&
		y >= 0 &&
		y < viewport->h)
	{
		DrawPixel(
			screen,
			color,
			x + viewport->x,
			y + viewport->y
		);
	}
}

#endif // !_CR_DRAW_H_
