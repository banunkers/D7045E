#include <glm/vec2.hpp>
#include "types.h"

#ifndef TTTREE_H
#define TTTREE_H

struct Edge {
	Point p0;
	Point p1;

	Edge(Point p0, Point p1) :
		p0(p0), p1(p1) {}
	
	bool leftOf(Point p) {
		return (p1.x - p0.x) * (p.y - p0.y) > (p1.y - p0.y) * (p.x - p0.x);
	}
};

struct Triangle {
	Point p0, p1, p2;

	Triangle(Point p0, Point p1, Point p2) :
		p0(p0), p1(p1), p2(p2) {}

	PointSet get() {
		return {p0, p1, p2};
	}
};

struct Node {
	Node *parent;

	virtual ~Node() {};

	Node(Node *parent) :
		parent(parent) {};
};

struct Leaf : Node {
	Triangle *triangle;

	Leaf(Triangle *triangle, Node *parent) :
		triangle(triangle), Node(parent) {}
};

struct BNode : Node {
	Edge first, mid, last;
	Node *lst, *rst;

	BNode(Point c, Point p0, Point p1, Point p2, Node *parent) :
		first(Edge(p0, c)),
		mid(Edge(p1, c)),
		last(Edge(p2, c)),
		lst(nullptr),
		rst(nullptr),
		Node(parent) {}
};

struct TNode : Node {
	Edge first, mid, last;
	Node *lst, *mst, *rst;

	TNode(Point c, Point p0, Point p1, Point p2) :
		first(Edge(p0, c)),
		mid(Edge(p1, c)),
		last(Edge(p2, c)),
		lst(nullptr),
		mst(nullptr),
		rst(nullptr),
		Node(nullptr) {}
};

#endif