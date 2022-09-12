#ifndef _CR_MATH_H_
#define _CR_MATH_H_

#define _USE_MATH_DEFINES

#include <stdbool.h>
#include <assert.h>
#include <math.h>

// Data types
typedef struct Point2D {
	double x;
	double y;
} Point2D;

typedef struct Vector2D {
	double x;
	double y;
} Vector2D;

typedef struct LineSegment2D {
	Point2D p1;
	Point2D p2;
} LineSegment2D;

typedef struct Frame2D {
	Point2D position;
	double theta;
} Frame2D;

// Function defs
extern Vector2D FindLookVector(const Vector2D* const worldForward, double theta);
extern bool DoesRayIntersectLine(
	const Point2D* const rayOrigin,
	const Vector2D* const rayDirection,
	const LineSegment2D* const lineSegment,
	double* const distanceToLine,
	Point2D* const intersectionPoint
);

// Inlined definitions
static inline int Sign(int value)
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

static inline double ToRad(double deg)
{
	return (deg * M_PI) / 180.0;
}

static inline double ToDeg(double rad)
{
	return (rad * 180.0) / M_PI;
}

static inline double Point2DLength(const Point2D point)
{
	return sqrt((point.x * point.x) + (point.y * point.y));
}

static inline Point2D Vec2DToPoint2D(const Vector2D vector)
{
	Point2D point =
	{
		.x = vector.x,
		.y = vector.y
	};

	return point;
}

static inline Point2D AddPoint2Ds(const Point2D p1, const Point2D p2)
{
	Point2D resultantPoint =
	{
		.x = p1.x + p2.x,
		.y = p1.y + p2.y
	};

	return resultantPoint;
}

static inline Point2D SubPoint2Ds(const Point2D p1, const Point2D p2)
{
	Point2D resultantPoint =
	{
		.x = p1.x - p2.x, .y = p1.y - p2.y
	};

	return resultantPoint;
}

static inline Point2D AddVec2DToPoint2D(const Point2D point, const Vector2D vector)
{
	Point2D resultantPoint =
	{
		.x = point.x + vector.x,
		.y = point.y + vector.y
	};

	return resultantPoint;
}

static inline Point2D SubVec2DFromPoint2D(const Point2D point, const Vector2D vector)
{
	Point2D resultantPoint =
	{
		.x = point.x - vector.x,
		.y = point.y - vector.y
	};

	return resultantPoint;
}

static inline double Vec2DLength(const Vector2D vector)
{
	return sqrt((vector.x * vector.x) + (vector.y * vector.y));
}

static inline Vector2D Vec2DNormalise(const Vector2D vector)
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

static inline Vector2D Vec2DBetween(const Point2D p1, const Point2D p2)
{
	Vector2D vector =
	{
		.x = p2.x - p1.x,
		.y = p2.y - p1.y
	};

	return vector;
}

static inline Vector2D Point2DToVec2D(const Point2D point)
{
	Vector2D vector =
	{
		.x = point.x,
		.y = point.y
	};

	return vector;
}

static inline Vector2D Vec2DMul(const Vector2D vector, double scalar)
{
	Vector2D resultantVector =
	{
		.x = vector.x * scalar,
		.y = vector.y * scalar
	};

	return resultantVector;
}

static inline Vector2D Vec2DDiv(const Vector2D vector, double scalar)
{
	Vector2D resultantVector =
	{
		.x = vector.x / scalar,
		.y = vector.y / scalar
	};

	return resultantVector;
}

static inline double Vec2DCross(const Vector2D v1, const Vector2D v2)
{
	return (v1.x * v2.y) - (v1.y * v2.x);
}

static inline double Vec2DDot(const Vector2D v1, const Vector2D v2)
{
	return (v1.x * v2.x) + (v1.y * v2.y);
}

static inline Frame2D CreateFrame2D(double x, double y, double theta)
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

#endif // !_CR_MATH_H_
