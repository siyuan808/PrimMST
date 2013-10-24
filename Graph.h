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
	int id; // the index in the vertex array of the graph
	int key; // current key when calculate the mst
	int mstParent; // parent id in the mst
	Color color; // Marker used for traversal
	unordered_map<int, int> edges;
	Vertex(int i): id(i){ key = INT_MAX; mstParent = -1; color = White;}
};

class Graph {  // represent a non directed graph

private:
	bool addEdge(int i, int j, int w);
	void initialize(int n);
	void dfsVisit(int v);
public:

	int nVertices; // number of vertices
	Vertex ** vertices; // an array of vertices
	int mstCost; // mst cost after calculated

	Graph();
	void build(int n, double d);
	void buildFull(int n);
	void build(const char *fileName);
	void print();
	// To check whether corrent graph is connected or not
	bool isConnected();
	// Get the weight on edge (u, v)
	int getWeight(int u, int v);
	// reset all the vertices before dfs and prim
	void traversalInitialize();
	// prim's algo to calculate the mst
	void primMST(MinQueue *q);
	void printMST();

	virtual ~Graph();
};

#endif /* GRAPH_H_ */
