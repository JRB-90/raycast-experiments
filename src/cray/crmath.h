#ifndef _CR_MATH_H_
#define _CR_MATH_H_

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
extern inline int Sign(int value);
extern inline double ToRad(double deg);
extern inline double ToDeg(double rad);
extern inline double Point2DLength(const Point2D vector);
extern inline Point2D Vec2DToPoint2D(const Vector2D vector);
extern inline Point2D AddPoint2Ds(const Point2D p1, const Point2D p2);
extern inline Point2D SubPoint2Ds(const Point2D p1, const Point2D p2);
extern inline Point2D AddVec2DToPoint2D(const Point2D point, const Vector2D vector);
extern inline Point2D SubVec2DFromPoint2D(const Point2D point, const Vector2D vector);
extern inline double Vec2DLength(const Vector2D vector);
extern inline Vector2D Vec2DNormalise(const Vector2D vector);
extern inline Vector2D Vec2DBetween(const Point2D p1, const Point2D p2);
extern inline Vector2D Point2DToVec2D(const Point2D point);
extern inline Vector2D Vec2DMul(const Vector2D vector, double scalar);
extern inline Vector2D Vec2DDiv(const Vector2D vector, double scalar);
extern inline double Vec2DCross(const Vector2D v1, const Vector2D v2);
extern inline double Vec2DDot(const Vector2D v1, const Vector2D v2);
extern inline Frame2D CreateFrame2D(double x, double y, double theta);

extern Vector2D FindLookVector(const Vector2D* const worldForward, double theta);
extern bool DoesRayIntersectLine(
	const Point2D* const rayOrigin,
	const Vector2D* const rayDirection,
	const LineSegment2D* const lineSegment,
	double* const distanceToLine,
	Point2D* const intersectionPoint
);

#endif // !_CR_MATH_H_
