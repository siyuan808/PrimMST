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
	virtual int extractMin() = 0; // Get the min and remove it from the queue
	virtual void decreaseKey(int v, int value) = 0; // Decrease the key in node v
	virtual bool isEmpty() = 0; // Check is the queue empty
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
	Type data; // An pointer to a vertex in a graph

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

	fnode *heap; // fibonacci heap's min pointer
	// an array help to look up the fnode according to the id of a vertex
	fnode* *f_map;
	// initialize the fib heap with the vertices in graph g
	void initialize(Graph *g);
	// insert a vertex into current fib heap
	fnode *insert(Type v);
	// merge two doubly linked list into one
	fnode *merge(fnode *a, fnode *b);
	// remove min and do the pairwise combine
	fnode *removeMin(fnode *n);
	// add a child fnode into a fnode
	void addChild(fnode *parent, fnode *child);
	// excise all the node in the list from their parent
	void removeAllFromParent(fnode *n);
	// excise fnode n from its parent and re-insert it into the top list
	fnode *cut(fnode *heap, fnode *n);
	// find the fnode according to the id of a vertex
	fnode *find(fnode *heap, int v);
	// delete every fnode and free the memory
	void deleteAll(fnode *n);
};

#endif /* MINQUEUE_H_ */
