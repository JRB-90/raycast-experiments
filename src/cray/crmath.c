#define _USE_MATH_DEFINES

#include "crmath.h"
#include <math.h>
#include <assert.h>

int Sign(int value)
{
	if (value < 0)
	{
		return -1;
	}
	else if (value > 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

double ToRad(double deg)
{
	return (deg * M_PI) / 180.0;
}

double ToDeg(double rad)
{
	return (rad * 180.0) / M_PI;
}

double Point2DLength(const Point2D point)
{
	return sqrt((point.x * point.x) + (point.y * point.y));
}

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

Vector2D Vec2DMul(const Vector2D vector, double scalar)
{
	Vector2D resultantVector =
	{
		.x = vector.x * scalar,
		.y = vector.y * scalar
	};

	return resultantVector;
}

Vector2D Vec2DDiv(const Vector2D vector, double scalar)
{
	Vector2D resultantVector =
	{
		.x = vector.x / scalar,
		.y = vector.y / scalar
	};

	return resultantVector;
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

Vector2D FindLookVector(Vector2D worldForward, double theta)
{
	double radTheta = ToRad(theta);

	Vector2D lookVector =
	{
		.x = (worldForward.x * cos(radTheta)) - (worldForward.y * sin(radTheta)),
		.y = (worldForward.y * cos(radTheta)) + (worldForward.x * sin(radTheta))
	};

	return Vec2DNormalise(lookVector);
}

bool DoesRayInterectLine(
	const Point2D rayOrigin, 
	const Vector2D rayDirection, 
	const LineSegment2D lineSegment,
	double* const distanceToLine,
	Point2D* const intersectionPoint)
{
	Vector2D dirNorm = Vec2DNormalise(rayDirection);
	Vector2D v1 = Vec2DBetween(lineSegment.p1, rayOrigin);
	Vector2D v2 = Vec2DBetween(lineSegment.p1, lineSegment.p2);
	Vector2D v3 = { .x = -dirNorm.y, .y = dirNorm.x };

	double dot = Vec2DDot(v2, v3);

	if (fabs(dot) < 0.000001)
	{
		return false;
	}

	double t1 = Vec2DCross(v2, v1) / dot;
	double t2 = Vec2DDot(v1, v3) / dot;

	if (t1 >= 0.0 &&
		(t2 >= 0.0 && t2 <= 1.0))
	{
		*distanceToLine = t1;
		Vector2D vecToIntersection = Vec2DMul(dirNorm, t1);
		Point2D intersect = AddVec2DToPoint2D(rayOrigin, vecToIntersection);
		intersectionPoint->x = intersect.x;
		intersectionPoint->y = intersect.y;

		return true;
	}

	return false;
}
