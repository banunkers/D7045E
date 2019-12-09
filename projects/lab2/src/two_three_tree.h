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

	PointSet toVec() {
		return {c, ci, cm, cj}; 
	}

	Node* insertPoint(Point &point) {
		printf("SEARCHING IN BINARY NODE\n");
		printf("point	: (%f, %f)\n", point.x, point.y);
		printf("c	: (%f, %f)\n", c.x, c.y);
		printf("ci	: (%f, %f)\n", ci.x, ci.y);
		printf("cm	: (%f, %f)\n", cm.x, cm.y);
		printf("cj	: (%f, %f)\n", cj.x, cj.y);

		Point *pointOnLine = 
			onLine(ci, c, point) ? &ci
				: onLine(cm, c, point) ? &cm 
				: onLine(cj, c, point) ? &cj 
				: nullptr;

		if (pointOnLine) {
			printf("Point on line: (%f, %f)\n", pointOnLine->x, pointOnLine->y);
			if (*pointOnLine == cm) {
				lst = lst->insertPoint(point);
				rst = rst->insertPoint(point);
			} else if (*pointOnLine == ci) {
				printf("ci point on line\n");
				lst = lst->insertPoint(point);
			} else if (*pointOnLine == cj) {
				printf("cj point on line\n");
				rst = rst->insertPoint(point);
			} else {
				printf("FUCKCK\n");
			}
		} else {
			if (leftOf(cm, c, ci)) {	// case 1 "ci left of cm->c"
				if (!leftOf(ci, c, point) && leftOf(cm, c, point)) {
					printf("left case1\n");
					lst = lst->insertPoint(point);
				} else {
					printf("in right case1\n");
					rst = rst->insertPoint(point);
				}
			} else { 	// case 2 "ci right of cm->c"
				if (!leftOf(ci, c, point) || leftOf(cm, c, point)) {
					printf("left case2\n");
					lst = lst->insertPoint(point);
				} else {
					printf("right case2\n");
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
		printf("SEARCHING IN TERNARY NODE\n");
		printf("point	: (%f, %f)\n", point.x, point.y);
		printf("c	: (%f, %f)\n", c.x, c.y);
		printf("ci	: (%f, %f)\n", ci.x, ci.y);
		printf("cm	: (%f, %f)\n", cm.x, cm.y);
		printf("cj	: (%f, %f)\n", cj.x, cj.y);

		if (leftOf(cm, c, point) && !leftOf(ci, c, point)) {
			printf("in left\n");
			lst = lst->insertPoint(point);
		} else if (!leftOf(cm, c, point) && leftOf(cj, c, point)) {
			printf("in mid\n");
			mst = mst->insertPoint(point);
		} else {
			printf("in right\n");
			rst = rst->insertPoint(point);
		}
		return this;
	}

	PointSet toVec() {
		return {c, ci, cm, cj};
	}
};

struct Leaf : Node {
	Triangle *triangle;

	Leaf(Triangle *triangle, Node *parent) :
		triangle(triangle), Node(parent) {}

	Node* insertPoint(Point &point) {
		TNode *newNode = new TNode(point, triangle->p0, triangle->p1, triangle->p2, parent);
		newNode->lst = new Leaf(new Triangle(triangle->p0, triangle->p1, point), newNode);
		newNode->mst = new Leaf(new Triangle(point, triangle->p1, triangle->p2), newNode);
		newNode->rst = new Leaf(new Triangle(triangle->p0, point, triangle->p2), newNode);

		printf("FOUND IN LEAF\n");
		printf("(%f, %f) -> (%f, %f) -> (%f, %f)\n", triangle->p0.x,triangle->p0.y,triangle->p1.x,triangle->p1.y,triangle->p2.x,triangle->p2.y);

		// Determine the sub-tree of parent leaf is located
		// auto lst = static_cast<void*>(parent->getLst());
		// auto mst = static_cast<void*>(parent->getMst());
		// auto lst = dynamic_cast<Leaf*>(parent->getLst());
		// auto mst = dynamic_cast<Leaf*>(parent->getMst());
		// auto currLeaf = static_cast<Leaf*>(this);
		// // Node currNode = *this;
		// if (currLeaf == lst) {
		// 	printf("Replacing parent lst\n");
		// 	parent->setLst(newNode);
		// } else if (currLeaf == mst) {
		// 	printf("Replacing parent mst\n");
		// 	parent->setMst(newNode);
		// } else {
		// 	printf("Replacing parent rst\n");
		// 	parent->setRst(newNode);
		// }
		return newNode;
	}
};

#endif