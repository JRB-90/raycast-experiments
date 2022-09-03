#ifndef _CR_DRAW_H_
#define _CR_DRAW_H_

#include "crdisplay.h"

inline void WritePixel(
	const Display* const display,
	int x,
	int y,
	Color color
);
void DrawClearColor(
	const Display* const display, 
	Color color
);
void DrawLine(
	const Display* const display,
	int x1,
	int y1,
	int x2, 
	int y2,
	Color color
);
void DrawRect(
	const Display* const display, 
	int x, 
	int y, 
	int w, 
	int h, 
	Color color
);
void DrawRectFilled(
	const Display* const display,
	int x,
	int y,
	int w,
	int h,
	Color color
);

#endif // !_CR_DRAW_H_
