#include "crdraw.h"

void DrawClearColor(const Display* const display, Color color)
{
	for (int i = 0; i < display->width; i++)
	{
		for (int j = 0; j < display->height; j++)
		{
			WritePixel(display, i, j, color);
		}
	}
}

inline void WritePixel(
	const Display* const display, 
	int x, 
	int y, 
	Color color)
{
	const int offset = (display->width * y * 4) + x * 4;
	display->pixels[offset + 0] = color.a;
	display->pixels[offset + 1] = color.b;
	display->pixels[offset + 2] = color.g;
	display->pixels[offset + 3] = color.r;
}
