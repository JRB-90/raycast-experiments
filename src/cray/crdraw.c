#include "crdraw.h"

inline void WritePixel(
	const Display* const display,
	const Color* const color,
	int x,
	int y)
{
	const int offset = (display->width * y * 4) + x * 4;
	display->pixels[offset + 0] = color->a;
	display->pixels[offset + 1] = color->b;
	display->pixels[offset + 2] = color->g;
	display->pixels[offset + 3] = color->r;
}

inline void WritePixelViewport(
	const Display* const display,
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
		WritePixel(
			display,
			color,
			x + viewport->x,
			y + viewport->y
		);
	}
}

void DrawClearColor(
	const Display* const display, 
	const Color* const color)
{
	for (int i = 0; i < display->width; i++)
	{
		for (int j = 0; j < display->height; j++)
		{
			WritePixel(display, color, i, j);
		}
	}
}

void DrawLine(
	const Display* const display,
	const Rect* const viewport,
	const Color* const color,
	int x1,
	int y1,
	int x2,
	int y2)
{
	int dx = x2 - x1;
	int dy = y2 - y1;
	int incX = Sign(dx);
	int incY = Sign(dy);
	dx = abs(dx);
	dy = abs(dy);

	if (dy == 0)
	{
		// Horizontal line
		for (int x = x1; x != x2 + incX; x += incX)
		{
			WritePixelViewport(
				display,
				viewport,
				color,
				x,
				y1
			);
		}
	}
	else if (dx == 0)
	{
		// Vertical line
		for (int y = y1; y != y2 + incY; y += incY)
		{
			WritePixelViewport(
				display,
				viewport,
				color,
				x1,
				y
			);
		}
	}
	else if (dx >= dy)
	{
		// Horizontal bias line
		int slope = 2 * dy;
		int error = -dx;
		int errorInc = -2 * dx;
		int y = y1;

		for (int x = x1; x != x2 + incX; x += incX)
		{
			WritePixelViewport(
				display,
				viewport,
				color,
				x,
				y
			);

			error += slope;
			if (error >= 0)
			{
				y += incY;
				error += errorInc;
			}
		}
	}
	else
	{
		// Vertical bias line
		int slope = 2 * dx;
		int error = -dy;
		int errorInc = -2 * dy;
		int x = x1;

		for (int y = y1; y != y2 + incY; y += incY)
		{
			WritePixelViewport(
				display,
				viewport,
				color,
				x,
				y
			);

			error += slope;
			if (error >= 0)
			{
				x += incX;
				error += errorInc;
			}
		}
	}
}

void DrawRect(
	const Display* const display,
	const Rect* const viewport,
	const Color* const color,
	int x, 
	int y, 
	int w, 
	int h)
{
	DrawLine(
		display,
		viewport,
		color,
		x,
		y,
		x + w - 1,
		y
	);

	DrawLine(
		display,
		viewport,
		color,
		x + w - 1,
		y,
		x + w - 1,
		y + h - 1
	);

	DrawLine(
		display,
		viewport,
		color,
		x + w - 1,
		y + h - 1,
		x,
		y + h - 1
	);

	DrawLine(
		display,
		viewport,
		color,
		x,
		y + h - 1,
		x,
		y
	);
}

void DrawRectFilled(
	const Display* const display,
	const Rect* const viewport,
	const Color* const color,
	int x, 
	int y,
	int w, 
	int h)
{
	for (int i = x; i < x + w; i++)
	{
		for (int j = y; j < y + h; j++)
		{
			WritePixelViewport(
				display,
				viewport,
				color,
				i,
				j
			);
		}
	}
}
