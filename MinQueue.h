/*
 * MinQueue.h
 *
 *  Created on: Oct 11, 2013
 *      Author: yahuihan
 */

#ifndef MINQUEUE_H_
#define MINQUEUE_H_

#include "Graph.h"

class Graph;

class MinQueue {
public:
	virtual int extractMin() = 0;
	virtual void decreaseKey(int v, int value) = 0;
	virtual bool isEmpty() = 0;
	virtual ~MinQueue() {}
};

class SimpleQueue : public MinQueue {
private:
	int elements;
	Graph *g;
public:
	SimpleQueue(Graph *gh);
	virtual ~SimpleQueue() {}

	bool isEmpty();
	int extractMin();
	void decreaseKey(int v, int value);
};

class FibonacciHeap : public MinQueue {
private:
	Graph *g;
public:
	FibonacciHeap(Graph *gh);
	virtual ~FibonacciHeap() {}

	void initialize();
	bool isEmpty();
	int extractMin();
	void decreaseKey(int v, int value);
};

#endif /* MINQUEUE_H_ */
