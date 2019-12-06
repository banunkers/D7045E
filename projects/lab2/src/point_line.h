#include "types.h"

#ifndef POINT_LINE_H
#define POINT_LINE_H

/**
 * Calculates if a point is left of a line through two points
 * @param a point on the line
 * @param b point on the line 
 * @param point the point
 **/
bool leftOf(Point &a, Point &b, Point &point) {
	return ((b.x - a.x) * (point.y - a.y)) > ((b.y - a.y) * (point.x - a.x));
}

#endif