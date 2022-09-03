#include "crdraw.h"

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

void DrawClearColor(
	const Display* const display, 
	Color color)
{
	for (int i = 0; i < display->width; i++)
	{
		for (int j = 0; j < display->height; j++)
		{
			WritePixel(display, i, j, color);
		}
	}
}

void DrawLine(
	const Display* const display,
	int x1, 
	int y1, 
	int x2, 
	int y2, 
	Color color)
{
	int x = x1;
	int y = y1;
	int dx = x2 - x1;
	int dy = y2 - y1;
	int p = 2 * dy - dx;

	while (x < x1)
	{
		if (p >= 0)
		{
			WritePixel(
				display,
				x,
				y,
				color
			);

			y = y + 1;
			p = p + 2 * dy - 2 * dx;
		}
		else
		{
			WritePixel(
				display,
				x,
				y,
				color
			);

			p = p + 2 * dy;
		}

		x = x + 1;
	}
}

void DrawRect(
	const Display* const display, 
	int x, 
	int y, 
	int w, 
	int h, 
	Color color)
{
	DrawLine(
		display,
		x,
		y,
		x + w,
		y,
		color
	);

	DrawLine(
		display,
		x + w,
		y,
		x + w,
		y + h,
		color
	);

	DrawLine(
		display,
		x + w,
		y + h,
		x,
		y + h,
		color
	);

	DrawLine(
		display,
		x,
		y + h,
		x,
		y,
		color
	);
}

void DrawRectFilled(
	const Display* const display,
	int x, 
	int y, 
	int w, 
	int h, 
	Color color)
{
	for (int i = x; i < w; i++)
	{
		for (int j = y; j < h; j++)
		{
			WritePixel(
				display,
				i,
				j,
				color
			);
		}
	}
}
