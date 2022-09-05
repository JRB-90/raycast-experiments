#ifndef _CR_DRAW_H_
#define _CR_DRAW_H_

#include "crtypes.h"

extern inline void WritePixel(
	const ScreenBuffer* const screen,
	const Color* const color,
	int x,
	int y
);
extern inline void WritePixelViewport(
	const ScreenBuffer* const screen,
	const Rect* const viewport,
	const Color* const color,
	int x,
	int y
);
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

#endif // !_CR_DRAW_H_
