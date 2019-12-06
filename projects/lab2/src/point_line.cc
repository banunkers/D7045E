#include "point_line.h"

bool leftOf(Point &a, Point &b, Point &point) {
	return ((b.x - a.x) * (point.y - a.y)) > ((b.y - a.y) * (point.x - a.x));
}