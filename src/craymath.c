#include "craymath.h"
#include <math.h>
#include <assert.h>

Point2D Vec2DToPoint2D(const Vector2D vector)
{
	Point2D point =
	{
		.x = vector.x,
		.y = vector.y
	};

	return point;
}

Point2D AddPoint2Ds(const Point2D p1, const Point2D p2)
{
	Point2D resultantPoint = 
	{ 
		.x = p1.x + p2.x, 
		.y = p1.y + p2.y 
	};

	return resultantPoint;
}

Point2D SubPoint2Ds(const Point2D p1, const Point2D p2)
{
	Point2D resultantPoint = 
	{ 
		.x = p1.x - p2.x, .y = p1.y - p2.y 
	};

	return resultantPoint;
}

Point2D AddVec2DToPoint2D(const Point2D point, const Vector2D vector)
{
	Point2D resultantPoint = 
	{ 
		.x = point.x + vector.x, 
		.y = point.y + vector.y 
	};

	return resultantPoint;
}

Point2D SubVec2DFromPoint2D(const Point2D point, const Vector2D vector)
{
	Point2D resultantPoint =
	{
		.x = point.x - vector.x,
		.y = point.y - vector.y
	};

	return resultantPoint;
}

double Vec2DLength(const Vector2D vector)
{
	return sqrt((vector.x * vector.x) + (vector.y * vector.y));
}

Vector2D Vec2DNormalise(const Vector2D vector)
{
	double length = Vec2DLength(vector);

	assert(length != 0.0);

	Vector2D normalisedVec =
		{
			.x = vector.x / length,
			.y = vector.y / length,
		};

	return normalisedVec;
}

Vector2D Vec2DBetween(const Point2D p1, const Point2D p2)
{
	Vector2D vector = 
	{
		.x = p2.x - p1.x, 
		.y = p2.y - p1.y
	};

	return vector;
}

Vector2D Point2DToVec2D(const Point2D point)
{
	Vector2D vector =
	{
		.x = point.x,
		.y = point.y
	};

	return vector;
}

double Vec2DCross(const Vector2D v1, const Vector2D v2)
{
	return (v1.x * v2.y) - (v1.y * v2.x);
}

double Vec2DDot(const Vector2D v1, const Vector2D v2)
{
	return (v1.x * v2.x) + (v1.y * v2.y);
}

Frame2D CreateFrame2D(double x, double y, double theta)
{
	Frame2D frame =
	{
		.position =
		{
			.x = x,
			.y = y
		},
		.theta = theta
	};

	return frame;
}

Point2D CalculateOffsetPoint2D(Frame2D frame, double offset)
{
	Point2D offsetPoint;
	double x = 0.0;
	double y = -offset;
	offsetPoint.x = (x * cos(frame.theta)) - (y * sin(frame.theta));
	offsetPoint.y = (y * cos(frame.theta)) + (x * sin(frame.theta));
	
	return offsetPoint;
}

bool DoesVecInterectLine(const Vector2D vector, const LineSegment2D line)
{
	

	return false;
}





//bool CalculateIntersection(Line l1, Line l2, Vertex* interection)
//{
//	double s1_x = l1.p2.x - l1.p1.x;
//	double s1_y = l1.p2.y - l1.p1.y;
//	double s2_x = l2.p2.x - l2.p1.x;
//	double s2_y = l2.p2.y - l2.p1.y;
//
//	double denom = -s2_x * s1_y + s1_x * s2_y;
//
//	if (denom == 0.0)
//	{
//		return false;
//	}
//
//	double s = (-s1_y * (l1.p1.x - l2.p1.x) + s1_x * (l1.p1.y - l2.p1.y)) / denom;
//	double t = (s2_x * (l1.p1.y - l2.p1.y) - s2_y * (l1.p1.x - l2.p1.x)) / denom;
//
//	if (s >= 0 &&
//		s <= 1 &&
//		t >= 0 &&
//		t <= 1)
//	{
//		interection->x = l1.p1.x + (t * s1_x);
//		interection->y = l1.p1.y + (t * s1_y);
//
//		return true;
//	}
//
//	return false;
//}
