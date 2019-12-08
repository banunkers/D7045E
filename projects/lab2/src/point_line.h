#include "types.h"

#ifndef POINT_LINE_H
#define POINT_LINE_H

/**
 * Calculates if a point is left of a line through two points
 * @param a point on the line
 * @param b point on the line 
 * @param point the point
 **/
bool leftOf(Point &a, Point &b, Point &point);

bool onLine(Point &a, Point &b, Point &point, bool insideLine);

#endif