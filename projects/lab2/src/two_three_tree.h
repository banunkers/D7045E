#include <glm/vec2.hpp>
#include <iostream>
#include "types.h"
#include "point_line.h"

#ifndef TWO_THREE_TREE_H
#define TWO_THREE_TREE_H

struct Triangle {
	Point p0, p1, p2;

	Triangle(Point p0, Point p1, Point p2) :
		p0(p0), p1(p1), p2(p2) {}

	PointSet toVec() {
		return {p0, p1, p2};
	}

	PointSet toEdgesVec() {
		return {p0, p1, p0, p2, p1, p2};
	}
};

struct Node {
	Node *parent;

	virtual ~Node(){}
	virtual Node* insertPoint(Point &point){}

	Node(Node *parent) :
		parent(parent) {};
};

struct BNode : Node {
	Point c, ci, cm, cj;
	Node *lst, *rst;

	BNode(Point c, Point ci, Point cm, Point cj, Node *parent) :
		c(c), ci(ci), cm(cm), cj(cj), lst(nullptr), rst(nullptr), Node(parent) {}

	Node* insertPoint(Point &point) {
		Point *pointOnLine = 
			onLine(ci, c, point) ? &ci
				: onLine(cm, c, point) ? &cm 
				: onLine(cj, c, point) ? &cj 
				: nullptr;

		if (pointOnLine) {
			if (!this->parent) {	// root
				lst = lst->insertPoint(point);
				rst = rst->insertPoint(point);
			} else {
				if (*pointOnLine == cm) {
					lst = lst->insertPoint(point);
					rst = rst->insertPoint(point);
				} else if (*pointOnLine == ci) {
					lst = lst->insertPoint(point);
				} else if (*pointOnLine == cj) {
					rst = rst->insertPoint(point);
				}
			}
		} else {
			if (leftOf(cm, c, ci)) {	// case 1 "ci left of cm->c"
				if (!leftOf(ci, c, point) && leftOf(cm, c, point)) {
					lst = lst->insertPoint(point);
				} else {
					rst = rst->insertPoint(point);
				}
			} else { 	// case 2 "ci right of cm->c"
				if (!leftOf(ci, c, point) || leftOf(cm, c, point)) {
					lst = lst->insertPoint(point);
				} else {
					rst = rst->insertPoint(point);
				}
			}
		}
		return this;
	}
};

struct TNode : Node {
	Point c, ci, cm, cj;
	Node *lst, *mst, *rst;

	TNode(Point c, Point ci, Point cm, Point cj, Node *parent) :
		c(c), ci(ci), cm(cm), cj(cj), lst(nullptr), mst(nullptr), rst(nullptr), Node(parent) {}

	virtual Node* insertPoint(Point &point) {
		Point *pointOnLine = 
			onLine(ci, c, point) ? &ci
				: onLine(cm, c, point) ? &cm 
				: onLine(cj, c, point) ? &cj
				: nullptr;

		if (pointOnLine) {
			if (*pointOnLine == cm) {
				lst = lst->insertPoint(point);
				mst = mst->insertPoint(point);
			} else if (*pointOnLine == ci) {
				lst = lst->insertPoint(point);
				rst = rst->insertPoint(point);
			} else {
				mst = mst->insertPoint(point);
				rst = rst->insertPoint(point);
			}
		} else {
			if (leftOf(cm, c, point) && !leftOf(ci, c, point)) {
				lst = lst->insertPoint(point);
			} else if (!leftOf(cm, c, point) && leftOf(cj, c, point)) {
				mst = mst->insertPoint(point);
			} else {
				rst = rst->insertPoint(point);
			}
		}
		return this;
	}
};

struct Leaf : Node {
	Triangle *triangle;

	Leaf(Triangle *triangle, Node *parent) :
		triangle(triangle), Node(parent) {}

	Node* insertPoint(Point &point) {
		Point* pointOnLine = 
			onLine(triangle->p0, triangle->p1, point) ? &triangle->p1
				: onLine(triangle->p1, triangle->p2, point) ? &triangle->p2
				: onLine(triangle->p2, triangle->p0, point) ? &triangle->p0
				: nullptr;

		if (pointOnLine) {
			BNode *bn;
			// To determine correct vertices for the node and leafs the line the point lies on needs to be regarded
			if (*pointOnLine == triangle->p1) { // p0->p1
				bn = new BNode(point, triangle->p1, triangle->p2, triangle->p0, parent);
				bn->rst = new Leaf(new Triangle(point, triangle->p2, triangle->p0), bn);
				bn->lst = new Leaf(new Triangle(point, triangle->p1, triangle->p2), bn);
			} else if (*pointOnLine == triangle->p2) {	// p1->p2
				bn = new BNode(point, triangle->p2, triangle->p0, triangle->p1, parent);
				bn->rst = new Leaf(new Triangle(point, triangle->p0, triangle->p1), bn);
				bn->lst = new Leaf(new Triangle(point, triangle->p2, triangle->p0), bn);
			} else { // p2->p0
				bn = new BNode(point, triangle->p0, triangle->p1, triangle->p2, parent);
				bn->rst = new Leaf(new Triangle(point, triangle->p1, triangle->p2), bn);
				bn->lst = new Leaf(new Triangle(point, triangle->p0, triangle->p1), bn);
			}
			return bn;
		}

		TNode *newNode = new TNode(point, triangle->p0, triangle->p1, triangle->p2, parent);
		newNode->lst = new Leaf(new Triangle(point, triangle->p0, triangle->p1), newNode);
		newNode->mst = new Leaf(new Triangle(point, triangle->p1, triangle->p2), newNode);
		newNode->rst = new Leaf(new Triangle(point, triangle->p2, triangle->p0), newNode);

		return newNode;
	}
};

#endif