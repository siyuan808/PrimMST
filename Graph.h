/*
 * Graph.h
 *
 *  Created on: Oct 11, 2013
 *      Author: yahuihan
 */

#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <iostream>
#include "MinQueue.h"
#include <tr1/unordered_map>
#include <limits.h>

using namespace std;
using namespace tr1;

enum Color { White, Black };

class MinQueue;

struct Vertex{
	int id;
	int key;
	int mstParent;
	Color color; // Marker used for traversal
	unordered_map<int, int> edges;
	Vertex(int i): id(i){ key = INT_MAX; mstParent = -1; color = White;}
};

class Graph {  // represent a non directed graph

	//private method
	bool addEdge(int i, int j, int w);
	void initialize(int n);

	void dfsVisit(int v);
public:

	int nVertices;
	Vertex ** vertices;

	int mstCost;

	Graph();
	void build(int n, double d);
	void buildFull(int n);
	void build(const char *fileName);
	void print();
	bool isConnected();

	int getWeight(int u, int v);
	void traversalInitialize();
	void primMST(MinQueue *q);
	void printMST();

	virtual ~Graph();
};

#endif /* GRAPH_H_ */
