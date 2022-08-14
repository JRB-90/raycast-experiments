#include "math.h"
#include <math.h>

Vertex CreateDefaultVertex()
{

	Vertex vertex;
	vertex.x = 0.0;
	vertex.y = 0.0;

	return vertex;
}

Vertex CreateVertex(double x, double y)
{
	Vertex vertex;
	vertex.x = x;
	vertex.y = y;

	return vertex;
}

Frame CreateDefaultFrame()
{
	Frame frame;
	frame.x = 0.0;
	frame.y = 0.0;
	frame.theta = 0.0;

	return frame;
}

Frame CreateFrame(double x, double y, double theta)
{
	Frame frame;
	frame.x = x;
	frame.y = y;
	frame.theta = theta;

	return frame;
}

Line CreateLine(Vertex p1, Vertex p2)
{
	Line line;
	line.p1 = p1;
	line.p2 = p2;

	return line;
}

Vertex CalculateOffsetPoint(Frame frame, double offset)
{
	Vertex offsetPoint;
	double x = 0.0;
	double y = -offset;
	offsetPoint.x = (x * cos(frame.theta)) - (y * sin(frame.theta));
	offsetPoint.y = (y * cos(frame.theta)) + (x * sin(frame.theta));

	return offsetPoint;
}
