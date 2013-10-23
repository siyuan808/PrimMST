/*
 * MinQueue.h
 *
 *  Created on: Oct 11, 2013
 *      Author: yahuihan
 */

#ifndef MINQUEUE_H_
#define MINQUEUE_H_

#include "Graph.h"
#include <stdlib.h>

class Graph;
struct Vertex;

class MinQueue {
public:
	virtual int extractMin() = 0;
	virtual void decreaseKey(int v, int value) = 0;
	virtual bool isEmpty() = 0;
	virtual ~MinQueue() {}
};

typedef Vertex* Type;

class SimpleQueue : public MinQueue {
private:
//	Graph *g;
	int n;
	Type *data;

public:
	SimpleQueue(Graph *gh);
	virtual ~SimpleQueue() {
		if(data != NULL) {
			for(int i=0; i<n; i++)
				data[i] = NULL;
		}
	}

	bool isEmpty();
	int extractMin();
	void decreaseKey(int v, int value);
};

struct fnode {
	Type data;

	//Used for circular doubly linked list of siblings
	fnode* left;
	fnode* right;

	fnode* child;
	fnode* parent; //Pointer to parent node
	int degree;

	/* True if node has lost a child since it became a child of its current parent.
	 * Set to false by remove min, which is the only operation that makes one node a child of another.
	 * Undefined for a root node.
	 */
	bool childCut;

	fnode(Type d) { // a single fibonacci node/heap
		data = d;
		left = right = this;
		degree=0;
		childCut=false;
		child=NULL;
		parent=NULL;
	}
};

class FibonacciHeap : public MinQueue {
public:
	FibonacciHeap(Graph *gh);
	virtual ~FibonacciHeap();

	bool isEmpty();
	int extractMin();
	void decreaseKey(int v, int value);

private:
	Graph *graph;
	fnode *heap;

	fnode* *f_map;

	void initialize(Graph *g);
	fnode *insert(Type v);
//	fnode *singleton(Type value);
	fnode *merge(fnode *a, fnode *b);
	fnode *removeMin(fnode *n);
	void addChild(fnode *parent, fnode *child);
	void unMarkAndunParentAll(fnode *n);
	fnode *cut(fnode *heap, fnode *n);
	void deleteAll(fnode *n);
	fnode *find(fnode *heap, int v);
};

#endif /* MINQUEUE_H_ */
