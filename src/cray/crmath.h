#ifndef _CRAYMATH_H_
#define _CRAYMATH_H_

#include <stdbool.h>

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
extern int Sign(int value);
extern double ToRad(double deg);
extern double ToDeg(double rad);
extern double Point2DLength(const Point2D vector);
extern Point2D Vec2DToPoint2D(const Vector2D vector);
extern Point2D AddPoint2Ds(const Point2D p1, const Point2D p2);
extern Point2D SubPoint2Ds(const Point2D p1, const Point2D p2);
extern Point2D AddVec2DToPoint2D(const Point2D point, const Vector2D vector);
extern Point2D SubVec2DFromPoint2D(const Point2D point, const Vector2D vector);
extern double Vec2DLength(const Vector2D vector);
extern Vector2D Vec2DNormalise(const Vector2D vector);
extern Vector2D Vec2DBetween(const Point2D p1, const Point2D p2);
extern Vector2D Point2DToVec2D(const Point2D point);
extern Vector2D Vec2DMul(const Vector2D vector, double scalar);
extern Vector2D Vec2DDiv(const Vector2D vector, double scalar);
extern double Vec2DCross(const Vector2D v1, const Vector2D v2);
extern double Vec2DDot(const Vector2D v1, const Vector2D v2);
extern Frame2D CreateFrame2D(double x, double y, double theta);

extern Vector2D FindLookVector(Vector2D worldForward, double theta);
extern bool DoesRayInterectLine(
	const Point2D rayOrigin, 
	const Vector2D rayDirection, 
	const LineSegment2D lineSegment,
	double* const distanceToLine,
	Point2D* const intersectionPoint
);

#endif // !_CRAYMATH_H_
