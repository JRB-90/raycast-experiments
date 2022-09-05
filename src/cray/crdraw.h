#ifndef _CR_DRAW_H_
#define _CR_DRAW_H_

#include "crdisplay.h"

inline void WritePixel(
	const ScreenBuffer* const screen,
	const Color* const color,
	int x,
	int y
);
inline void WritePixelViewport(
	const ScreenBuffer* const screen,
	const Rect* const viewport,
	const Color* const color,
	int x,
	int y
);
void DrawClearColor(
	const ScreenBuffer* const screen,
	const Color* const color
);
void DrawLine(
	const ScreenBuffer* const screen,
	const Rect* const viewport,
	const Color* const color,
	int x1,
	int y1,
	int x2, 
	int y2
);
void DrawRect(
	const ScreenBuffer* const screen,
	const Rect* const viewport,
	const Color* const color,
	int x, 
	int y, 
	int w, 
	int h
);
void DrawRectFilled(
	const ScreenBuffer* const screen,
	const Rect* const viewport,
	const Color* const color,
	int x,
	int y,
	int w,
	int h
);

#endif // !_CR_DRAW_H_
