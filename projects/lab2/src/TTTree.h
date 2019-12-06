#include <glm/vec2.hpp>
#include <iostream>
#include "types.h"
#include "point_line.h"

#ifndef TTTREE_H
#define TTTREE_H

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

	virtual ~Node() {};
	virtual Point getC();
	virtual Point getCm();
	virtual Point getCi();
	virtual Point getCj();

	virtual Node* getLst();
	virtual Node* getMst();
	virtual Node* getRst();
	virtual void setLst(Node* tree);
	virtual void setMst(Node* tree);
	virtual void setRst(Node* tree);

	virtual void insertPoint(Point &point) = 0;

	Node(Node *parent) :
		parent(parent) {};
};

struct BNode : Node {
	Point c, ci, cm, cj;
	Node *lst, *rst;

	BNode(Point c, Point ci, Point cm, Point cj, Node *parent) :
		c(c),
		ci(ci),
		cm(cm),
		cj(cj),
		lst(nullptr),
		rst(nullptr),
		Node(parent) {}
	
	virtual Point getC() {return c;}
	virtual Point getCm() {return cm;}
	virtual Point getCi() {return ci;}
	virtual Point getCj() {return cj;}

	virtual Node* getLst() {return lst;}
	virtual Node* getMst() {return nullptr;}
	virtual Node* getRst() {return rst;}
	virtual void setLst(Node* tree) {lst = tree;}
	virtual void setRst(Node* tree) {rst = tree;}

	PointSet toVec() {
		return {c, ci, cm, cj}; 
	}

	void insertPoint(Point &point) {
		printf("SEARCHING IN NODE\n");
		printf("point	: (%f, %f)\n", point.x, point.y);
		printf("c	: (%f, %f)\n", c.x, c.y);
		printf("ci	: (%f, %f)\n", ci.x, ci.y);
		printf("cm	: (%f, %f)\n", cm.x, cm.y);
		printf("cj	: (%f, %f)\n", cj.x, cj.y);
		Node* foundInNode;
		if (leftOf(cm, c, ci)) {	// case 1 "ci left of cm->c"
			if (!leftOf(ci, c, point) && leftOf(cm, c, point)) {
				printf("right case1\n");
				rst->insertPoint(point);
			}
			printf("in left case1\n");
			lst->insertPoint(point);
		} else { 	// case 2 "ci right of cm->c"
			if (!leftOf(ci, c, point) || leftOf(cm, c, point)) {
				printf("right case2\n");
				rst->insertPoint(point);
			}
			printf("left case2\n");
			lst->insertPoint(point);
		}
	}
};

struct TNode : Node {
	Point c, ci, cm, cj;
	Node *lst, *mst, *rst;

	TNode(Point c, Point ci, Point cm, Point cj, Node *parent) :
		c(c),
		ci(ci),
		cm(cm),
		cj(cj),
		lst(nullptr),
		mst(nullptr),
		rst(nullptr),
		Node(parent) {}
	
	virtual Point getC() {return c;}
	virtual Point getCm() {return cm;}
	virtual Point getCi() {return ci;}
	virtual Point getCj() {return cj;}

	virtual Node* getLst() {return lst;}
	virtual Node* getMst() {return mst;}
	virtual Node* getRst() {return rst;}
	virtual void setLst(Node* tree) {lst = tree;}
	virtual void setMst(Node* tree) {mst = tree;}
	virtual void setRst(Node* tree) {rst = tree;}

	virtual void insertPoint(Point &point) {
		return;
	}

	PointSet toVec() {
		return {c, ci, cm, cj};
	}
};

struct Leaf : Node {
	Triangle *triangle;

	Leaf(Triangle *triangle, Node *parent) :
		triangle(triangle), Node(parent) {}

	void insertPoint(Point &point) {
		auto c = parent->getC();
		auto cm = parent->getCm();
		auto cj = parent->getCj();

		TNode *newNode = new TNode(point, c, cm, cj, parent);
		newNode->lst = new Leaf(new Triangle(c, cm, point), newNode);
		newNode->mst = new Leaf(new Triangle(point, cm, cj), newNode);
		newNode->rst = new Leaf(new Triangle(c, point, cj), newNode);

		// Determine the sub-tree of parent leaf is located
		if (parent->getLst() == this) { 
			parent->setLst(newNode);
		} else if (parent->getMst() == this) {
			parent->setMst(newNode);
		} else {
			parent->setRst(newNode);
		}
	}
};

#endif