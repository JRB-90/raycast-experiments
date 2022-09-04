#ifndef _CR_DRAW_H_
#define _CR_DRAW_H_

#include "crdisplay.h"

inline void WritePixel(
	const Display* const display,
	const Color* const color,
	int x,
	int y
);
inline void WritePixelViewport(
	const Display* const display,
	const Rect* const viewport,
	const Color* const color,
	int x,
	int y
);
void DrawClearColor(
	const Display* const display, 
	const Color* const color
);
void DrawLine(
	const Display* const display,
	const Rect* const viewport,
	const Color* const color,
	int x1,
	int y1,
	int x2, 
	int y2
);
void DrawRect(
	const Display* const display,
	const Rect* const viewport,
	const Color* const color,
	int x, 
	int y, 
	int w, 
	int h
);
void DrawRectFilled(
	const Display* const display,
	const Rect* const viewport,
	const Color* const color,
	int x,
	int y,
	int w,
	int h
);

#endif // !_CR_DRAW_H_
