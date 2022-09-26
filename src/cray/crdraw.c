#define _USE_MATH_DEFINES

#include "crdraw.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "crmath.h"

void DrawClearColor(
	const ScreenBuffer* const screen,
	const Color* const color)
{
	if (screen->bytesPP == 4)
	{
		const int offset = screen->offset / screen->bytesPP;
		const int pixelCount = screen->width * screen->height;
		uint32_t* buf = (uint32_t*)screen->pixels;

		if (screen->colorFormat == CF_ARGB)
		{
			for (int i = offset; i < pixelCount + offset; i++)
			{
				buf[i] = ToUint32ARGBColor(color);
			}
		}
		else if (screen->colorFormat == CF_RGBA)
		{
			for (int i = offset; i < pixelCount + offset; i++)
			{
				buf[i] = ToUint32RGBAColor(color);
			}
		}
	}
	else if (screen->bytesPP == 2)
	{
		const int offset = screen->offset / screen->bytesPP;
		const int pixelCount = screen->width * screen->height;
		uint16_t* buf = (uint16_t*)screen->pixels;
		uint16_t c = ToUint16RGB565Color(color);

		for (int i = offset; i < pixelCount + offset; i++)
		{
			buf[i] = c;
		}
	}
}

void DrawLine(
	const ScreenBuffer* const screen,
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
			DrawPixelViewport(
				screen,
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
			DrawPixelViewport(
				screen,
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
			DrawPixelViewport(
				screen,
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
			DrawPixelViewport(
				screen,
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
	const ScreenBuffer* const screen,
	const Rect* const viewport,
	const Color* const color,
	int x, 
	int y, 
	int w, 
	int h)
{
	for (int i = x; i < x + w - 1; i++)
	{
		DrawPixelViewport(
			screen,
			viewport,
			color,
			i,
			y
		);

		DrawPixelViewport(
			screen,
			viewport,
			color,
			i,
			y + h - 1
		);
	}

	for (int i = y; i < y + h - 1; i++)
	{
		DrawPixelViewport(
			screen,
			viewport,
			color,
			x,
			i
		);

		DrawPixelViewport(
			screen,
			viewport,
			color,
			x + w - 1,
			i
		);
	}
}

void DrawRectFilled(
	const ScreenBuffer* const screen,
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
			DrawPixelViewport(
				screen,
				viewport,
				color,
				i,
				j
			);
		}
	}
}
