#include "triangle_soup.h"
#include <iostream>
#include <glm/glm.hpp>
#include <algorithm>
#include <tuple>

/**
 * Picks a point inside of the convex hull and closest to the origin, if no points 
 * exists inside the convex hull the point (on the convex hull) closest to the origin will be picked.
 * @param set the point set
 * @param cHull the convex hull of the point set
 **/
Point pickPoint(PointSet &set, PointSet &insideCHull) {
	Point closest;
	float closestDist = 2;
	Point origin = Point(0, 0);

	if (insideCHull.size() > 0) {
		for (const auto &point: insideCHull) {
			auto dist = glm::distance(origin, point);
			if (dist < closestDist) {
				closest = point;
				closestDist = dist;
			}
		}
	} else {
		for (const auto &point: set) {
			auto dist = glm::distance(origin, point);
			if (dist < closestDist) {
				closest = point;
				closestDist = dist;
			}
		}
	}
	
	return closest;
}

/**
 * Sorts a point set by the x-coordinate (starting with largest) and if tie by the y-coordinate)
 * @param pointSet the point set
 **/
void sortPointSet(PointSet &set) {
	std::sort(set.begin(), set.end(), [](const Point &p0, const Point &p1) {
		if (p0.x == p1.x) {
			return p0.y > p1.y;
		}
		return p0.x > p1.x;
	});
}

/**
 * Calculates if a point is left of a line through two points
 * @param a point on the line
 * @param b point on the line 
 * @param point the point
 **/
bool leftOf(Point &a, Point &b, Point &point) {
	return ((b.x - a.x) * (point.y - a.y)) > ((b.y - a.y) * (point.x - a.x));
}

/**
 * Calculates the convex hull of a point set using Andrew's algorithm,
 * the first point of the convex hull will also be the last in the resulting point set
 * @param set a point set
 * @returns a point set containing the points on the convex hull
 **/
PointSet convexHull(PointSet &set) {
	if (set.size() <= 3) return set;

	// Sort the point set by x-coordinate
	auto sortedSet = set;
	sortPointSet(sortedSet);

	// Calculate upper hull
	auto upper = PointSet();
	for (int i = 0; i < sortedSet.size(); i++) {
		// while the hull contains at least two points and the considered point lies to the left of the line through last two points
		// of the hull, pop from the hull to repair it
		while (upper.size() > 1 && leftOf(upper[upper.size() - 2], upper[upper.size() - 1], sortedSet[i])) {
			upper.pop_back();
		}
		upper.push_back(sortedSet[i]);
	}

	// Calculate lower hull
	auto lower = PointSet();
	for (int i = sortedSet.size() - 1; i  >= 0; i--) {
		while (lower.size() > 1 && leftOf(lower[lower.size() - 2], lower[lower.size() - 1], sortedSet[i])) {
			lower.pop_back();
		}
		lower.push_back(sortedSet[i]);
	}

	// Remove last element of upper hull do not get duplicate when concat with lower
	upper.pop_back();
	upper.insert(upper.end(), lower.begin(), lower.end());

	return upper;
}

/**
 * Extracts triangles contained in the leafs of a 2-3 search tree
 * @param node the root of the 2-3 search tree
 * @returns a vector containing the vertices create the triangles
 **/
PointSet extractTriangles(Node *node) {
	if (Leaf *leaf = dynamic_cast<Leaf*>(node)) {	// leaf
		auto triangle = leaf->triangle->get();
		return {triangle[0], triangle[1], triangle[0], triangle[2]};
	} else if (BNode *bn = dynamic_cast<BNode*>(node)) {	// binary
		auto lTriangles = extractTriangles(bn->lst);
		auto rTriangles = extractTriangles(bn->rst);
		lTriangles.insert(lTriangles.end(), rTriangles.begin(), rTriangles.end());
		return lTriangles;
	} else {	// ternary
		TNode *tn = dynamic_cast<TNode*>(node); 
		auto lTriangles = extractTriangles(tn->lst);
		auto mTriangles = extractTriangles(tn->mst);
		auto rTriangles = extractTriangles(tn->rst);
		lTriangles.insert(lTriangles.end(), mTriangles.begin(), mTriangles.end());
		lTriangles.insert(lTriangles.end(), rTriangles.begin(), rTriangles.end());
		return lTriangles;
	}
}

/**
 * Builds an initial 2-3 search tree of the convex hull and a point c
 **/
Node* buildTree(Point &c, PointSet cHull, Node *parent) {
	if (cHull.size() == 2) {
		return new Leaf(new Triangle(c, cHull[0], cHull[1]), parent);
	}

	auto medianIndex = cHull.size() / 2;
	BNode *bn = new BNode(c, cHull[0], cHull[medianIndex], cHull[cHull.size() - 1], parent);

	// compute sub tress
	bn->lst = buildTree(c, PointSet(cHull.begin(), cHull.begin() + medianIndex + 1), bn);
	bn->rst = buildTree(c, PointSet(cHull.begin() + medianIndex, cHull.end()), bn);
	
	return bn;
}

/**
 * Searches through a 2-3 search tree and locates where in the tree a point lies
 * @param tree the 2-3 search tree
 * @param point the point to locate
 * @returns the leaf in which the point is located
 **/
Node* searchTree(Node *tree, Point &point) {
	if (Leaf *leaf = dynamic_cast<Leaf*>(tree)) {
		return leaf;
	} else if (BNode *bn = dynamic_cast<BNode*>(tree)) {
		printf("SEARCHING IN NODE\n");
		printf("point	: (%f, %f)\n", point.x, point.y);
		printf("c	: (%f, %f)\n", bn->c.x, bn->c.y);
		printf("ci	: (%f, %f)\n", bn->ci.x, bn->ci.y);
		printf("cm	: (%f, %f)\n", bn->cm.x, bn->cm.y);
		printf("cj	: (%f, %f)\n", bn->cj.x, bn->cj.y);

		// if (bn->mid.leftOf(bn->first.p0)) {	// case 1 "ci left of cm->c"
		// 	if (!bn->first.leftOf(point) && bn->mid.leftOf(point)) {
		// 		return searchTree(bn->lst, point);
		// 	}
		// 	// return searchTree(bn->rst, point);
		// }

		// // case 2 "ci right of cm->c"
		// if (!bn->mid.leftOf(point) || bn->first.leftOf(point)) {
		// 	return searchTree(bn->lst, point);
		// }
		// return searchTree(bn->rst, point);
	}
}

void insertPoint(Point &point) {

}

std::tuple<PointSet, Point, PointSet> triangleSoup(PointSet &set) {
	auto cHull = convexHull(set);
	
	if (set.size() <= 3) {
		return std::make_tuple(cHull, Point(), PointSet());
	}

	// Create a point set of points inside the convex hull
	auto insideCHull = PointSet();
	for (const auto &point: set) {
		if (std::find(cHull.begin(), cHull.end(), point) == cHull.end()) {
			insideCHull.push_back(point);
		}
	}

	// pick point c inside the convex hull to construct the inital triangle fan from
	auto c = pickPoint(set, insideCHull);
	auto tree = buildTree(c, cHull, nullptr);
	
	if (insideCHull.size() > 0) {
		insideCHull.erase(std::find(insideCHull.begin(), insideCHull.end(), c));
	}

	for (auto &point: insideCHull) {
		auto location = searchTree(tree, point);
		// Leaf *leaf = dynamic_cast<Leaf*>(location);
		printf("Found node in triangle\n");
		// printf("(%f, %f) \n (%f, %f) \n (%f, %f) \n", leaf->triangle->p0.x, leaf->triangle->p0.y, leaf->triangle->p1.x, leaf->triangle->p1.y, leaf->triangle->p2.x, leaf->triangle->p2.y);
	}

	auto triangulation = extractTriangles(tree);
	
	return std::make_tuple(cHull, c, triangulation);
}