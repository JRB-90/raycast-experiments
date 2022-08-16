#ifndef _MATH_H_
#define _MATH_H_

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
extern Point2D Vec2DToPoint2D(const Vector2D vector);
extern Point2D AddPoint2Ds(const Point2D p1, const Point2D p2);
extern Point2D SubPoint2Ds(const Point2D p1, const Point2D p2);
extern Point2D AddVec2DToPoint2D(const Point2D point, const Vector2D vector);
extern Point2D SubVec2DFromPoint2D(const Point2D point, const Vector2D vector);
extern double Vec2DLength(const Vector2D vector);
extern Vector2D Vec2DNormalise(const Vector2D vector);
extern Vector2D Vec2DBetween(const Point2D p1, const Point2D p2);
extern Vector2D Point2DToVec2D(const Point2D point);
extern double Vec2DCross(const Vector2D v1, const Vector2D v2);
extern double Vec2DDot(const Vector2D v1, const Vector2D v2);
extern Frame2D CreateFrame2D(double x, double y, double theta);

extern Point2D CalculateOffsetPoint2D(Frame2D frame, double offset);
extern bool DoesVecInterectLine(const Vector2D vector, const LineSegment2D line);

#endif // !_MATH_H_
