#ifndef _CR_DRAW_H_
#define _CR_DRAW_H_

#include "crdisplay.h"

inline void WritePixel(
	const Display* const display,
	int x,
	int y,
	Color color
);
void DrawClearColor(const Display* const display, Color color);

#endif // !_CR_DRAW_H_
