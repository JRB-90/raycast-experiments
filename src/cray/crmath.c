#include "crmath.h"

Vector2D FindLookVector(const Vector2D* const worldForward, double theta)
{
	double radTheta = ToRad(theta);

	Vector2D lookVector =
	{
		.x = (worldForward->x * cos(radTheta)) - (worldForward->y * sin(radTheta)),
		.y = (worldForward->y * cos(radTheta)) + (worldForward->x * sin(radTheta))
	};

	return Vec2DNormalise(lookVector);
}

bool DoesRayIntersectLine(
	const Point2D* const rayOrigin, 
	const Vector2D* const rayDirection,
	const LineSegment2D* const lineSegment,
	double* const distanceToLine,
	Point2D* const intersectionPoint)
{
	Vector2D dirNorm = Vec2DNormalise(*rayDirection);
	Vector2D v1 = Vec2DBetween(lineSegment->p1, *rayOrigin);
	Vector2D v2 = Vec2DBetween(lineSegment->p1, lineSegment->p2);
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
		Point2D intersect = AddVec2DToPoint2D(*rayOrigin, vecToIntersection);
		intersectionPoint->x = intersect.x;
		intersectionPoint->y = intersect.y;

		return true;
	}

	return false;
}
