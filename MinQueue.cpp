/*
 * MinQueue.cpp
 *
 *  Created on: Oct 11, 2013
 *      Author: yahuihan
 */

#include "MinQueue.h"

// --------------------------------SimpleQueue------------------------------------
SimpleQueue::SimpleQueue(Graph *gh){
	g = gh;
	elements = g->nVertices;
}

bool SimpleQueue::isEmpty() {
	return elements <= 0;
}

int SimpleQueue::extractMin() {
	int res = -1;
	int minKey = INT_MAX;
	for(int i = 0; i < g->nVertices; i++ ) {
		if(g->vertices[i]->color == White && g->vertices[i]->key < minKey) {
			res = i;
			minKey = g->vertices[i]->key;
		}
	}
	elements--;
	return res;
}

void SimpleQueue::decreaseKey(int v, int value) {
	g->vertices[v]->key = value;
}

// --------------------------------Fibonacci Heap-------------------------------
FibonacciHeap::FibonacciHeap(Graph *gh) {
	g = gh;

}

void FibonacciHeap::initialize(){

}

bool FibonacciHeap::isEmpty() {

}

int FibonacciHeap::extractMin() {

}

void FibonacciHeap::decreaseKey(int v, int value) {

}
