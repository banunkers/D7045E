#include "triangle_soup.h"
#include "point_line.h"
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
	float closestDist = 100;
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
 * Sorts a point set by the x-coordinate (from largest to smallest) and if tie by the y-coordinate
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

	// Calculate lower hull
	auto lower = PointSet();
	for (int i = 0; i < sortedSet.size(); i++) {
		while (lower.size() > 1 && leftOf(lower[lower.size() - 2], lower[lower.size() - 1], sortedSet[i])) {
			lower.pop_back();
		}
		lower.push_back(sortedSet[i]);
	}
	
	// Calculate upper hull
	auto upper = PointSet();
	for (int i = sortedSet.size() - 1; i >= 0; i--) {
		// while the hull contains at least two points and the considered point lies to the left of the line through last two points
		// of the hull, pop from the hull to repair it
		while (upper.size() > 1 && leftOf(upper[upper.size() - 2], upper[upper.size() - 1], sortedSet[i])) {
			upper.pop_back();
		}
		upper.push_back(sortedSet[i]);
	}

	// Remove last element of lower hull do not get duplicate when concat with upper
	lower.pop_back();
	lower.insert(lower.end(), upper.begin(), upper.end());
	return lower;
}

/**
 * Extracts triangles contained in the leafs of a 2-3 search tree
 * @param node a node in the 2-3 search tree
 * @returns a vector containing the vertices of the triangles
 **/
PointSet extractTriangles(Node *node) {
	if (Leaf *leaf = dynamic_cast<Leaf*>(node)) {	// leaf
		return leaf->triangle->toVec();
	} else if (BNode *bn = dynamic_cast<BNode*>(node)) {	// binary
		auto lTriangles = extractTriangles(bn->lst);
		auto rTriangles = extractTriangles(bn->rst);
		lTriangles.insert(lTriangles.end(), rTriangles.begin(), rTriangles.end());
		return lTriangles;
	} else if (TNode *tn = dynamic_cast<TNode*>(node)){
		auto lTriangles = extractTriangles(tn->lst);
		auto mTriangles = extractTriangles(tn->mst);
		auto rTriangles = extractTriangles(tn->rst);
		lTriangles.insert(lTriangles.end(), mTriangles.begin(), mTriangles.end());
		lTriangles.insert(lTriangles.end(), rTriangles.begin(), rTriangles.end());
		return lTriangles;
	}
}

/**
 * Extracts the edges contained in the leafs of a 2-3 search tree
 **/
PointSet extractEdges(Node *node) {
	if (Leaf *leaf = dynamic_cast<Leaf*>(node)) {	// leaf
		return leaf->triangle->toEdgesVec();
	} else if (BNode *bn = dynamic_cast<BNode*>(node)) {	// binary
		auto lEdges = extractEdges(bn->lst);
		auto rEdges = extractEdges(bn->rst);
		lEdges.insert(lEdges.end(), rEdges.begin(), rEdges.end());
		return lEdges;
	} else if (TNode *tn = dynamic_cast<TNode*>(node)){
		auto lEdges = extractEdges(tn->lst);
		auto mEdges = extractEdges(tn->mst);
		auto rEdges = extractEdges(tn->rst);
		lEdges.insert(lEdges.end(), mEdges.begin(), mEdges.end());
		lEdges.insert(lEdges.end(), rEdges.begin(), rEdges.end());
		return lEdges;
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
 * Calculates a triangle soup given a point set consisting of n >= 3 points
 * @returns a tuple containing the convex hull, triangle vertices and edges of the triangle soup
 **/
std::tuple<PointSet,PointSet, PointSet> triangleSoup(PointSet &set) {
	auto cHull = convexHull(set);
	std::reverse(cHull.begin(), cHull.end()); 	// make CCW
	
	if (set.size() <= 3) {
		auto edges = PointSet{cHull[0], cHull[1], cHull[0], cHull[2], cHull[1], cHull[2]};
		return std::make_tuple(cHull, cHull, edges);
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
	if (insideCHull.size() > 0) {
		insideCHull.erase(std::find(insideCHull.begin(), insideCHull.end(), c));
	}

	// build initial 2-3 search three containing the convex hull and point c
	auto tree = buildTree(c, cHull, nullptr);

	// Insert every point inside the convex hull in the 2-3 search tree
	for (auto &point: insideCHull) {
		tree->insertPoint(point);
	}

	auto triangles = extractTriangles(tree);
	auto edges = extractEdges(tree);
	
	cHull.pop_back(); // remove duplicate
	return std::make_tuple(cHull, triangles, edges);
}