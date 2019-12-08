#include "point_line.h"

bool leftOf(Point &a, Point &b, Point &point) {
	return ((b.x - a.x) * (point.y - a.y)) > ((b.y - a.y) * (point.x - a.x));
}

bool onLine(Point &a, Point &b, Point &point, bool insideLine) {
	if (insideLine) {
		return (((b.x - a.x) * (point.y - a.y)) == ((b.y - a.y) * (point.x - a.x))) && (point.x > a.x && point.x < b.x && point.y > a.y && point.y < b.y);
	}
	return ((b.x - a.x) * (point.y - a.y)) == ((b.y - a.y) * (point.x - a.x));
}