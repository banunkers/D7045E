#include "point_line.h"
#include <glm/glm.hpp>

bool leftOf(Point &a, Point &b, Point &point) {
	return ((b.x - a.x) * (point.y - a.y)) > ((b.y - a.y) * (point.x - a.x));
}

bool onLine(Point &a, Point &b, Point &point) {
	return (((b.x - a.x) * (point.y - a.y)) == ((b.y - a.y) * (point.x - a.x)))
		&& (glm::normalize(a - b) == glm::normalize(point - b));
}