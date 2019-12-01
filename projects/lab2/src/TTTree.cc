#include "config.h"
#include <glm/vec2.hpp>

typedef glm::vec2 Point;

struct Edge {
	Point p0;
	Point p1;

	Edge(Point p0, Point p1) {
		this->p0 = p0;
		this->p1 = p1;
	}
	
	Edge() {};

	bool leftOf(Point p) {
		return (p1.x - p0.x) * (p.y - p0.y) > (p1.y - p0.y) * (p.x - p0.x);
	}
};

struct BNode {
	Edge first, mid, last;
	Node &left, &right;

	BNode(Point c, Point p0, Point p1, Point p2) {
		this->first = Edge(c, p0);
		this->mid = Edge(c, p1);
		this->last = Edge(c, p2);
		this->left = nullptr;
		this->right = nullptr;
	}

};

struct TNode {
	Edge first, mid, last;
	Node &left, &mid, &right;

	TNode(Point c, Point p0, Point p1, Point p2) {
		this->first = Edge(c, p0);
		this->mid = Edge(c, p1);
		this->last = Edge(c, p2);
		this->left = nullptr;
		this->right = nullptr;
		this->mid = nullptr;
	}
};

enum struct Node {TNode, BNode};