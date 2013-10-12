/*
 * Graph.cpp
 *
 *  Created on: Oct 11, 2013
 *      Author: yahuihan
 */

#include "Graph.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

Graph::Graph() {
	// TODO Auto-generated constructor stub
	mstCost = -1;
}
//---------------------------------------Build a Graph----------------------------------
void Graph::buildFull (int n){
	initialize(n);
	int i, j, w;
	srand(time(NULL));

	for(i = 0; i < nVertices; i++) {
		for(j = i+1; j < nVertices; j++) {
			w = rand() % 1000;
			addEdge(i, j, w);
		}
	}
}

void Graph::build(int n, double d) {
	// This means the density is almost 1, create a full graph
	if(d > 0.99) {
		buildFull(n);
		return;
	}

	initialize(n);
	int m = d * n * (n - 1) / 2;
	int e = 0;

	srand(time(NULL));
	int i, j, w;
	while(e <= m) {
		i = rand() % n;
		j = rand() % n;
		w = rand() % 1000;
		if(addEdge(i, j, w)) // (i, j) not already in there
			e++;
	}
	while(! isConnected()) {
		i = rand() % n;
		j = rand() % n;
		w = rand() % 1000;
		if(addEdge(i, j, w))
			e++;
	}
	cout <<"Actual density is : " << (double) 2 * e/(n * (n-1)) <<endl;
}

void Graph::build(const char *fileName) {
	FILE *fp = fopen(fileName, "r");
	if( !fp ) {
		cout <<"Can not open file " <<fileName <<endl;
		exit(1);
	}
	int n, m;
	fscanf(fp, "%d", &n);
	fscanf(fp, "%d", &m);
	initialize(n);
	int i, j, w;
	int e = 0;
	while(e < m) {
		fscanf(fp, "%d", &i);
		fscanf(fp, "%d", &j);
		fscanf(fp, "%d", &w);
		addEdge(i, j, w);
		e++;
	}
	fclose(fp);
}

bool Graph::addEdge(int i, int j, int w ) {
	Vertex *v = vertices[i];
	for(int k = 0; k < v->edges.size(); k++) {
		//Edge (i, j) already in the graph
		if(v->edges[k].index == j)
			return false;
	}
	// e(i, j) not in the graph, then add
	v->edges.push_back(EdgeEnd(j, w));
	vertices[j]->edges.push_back(EdgeEnd(i, w));
	return true;
}

void Graph::initialize(int n) {
	this->nVertices  = n;
	this->vertices = (Vertex * *) malloc( n * sizeof(Vertex*));
	for(int i=0; i<n; i++)
		vertices[i] = new Vertex();
}

//------------------------------------DFS and check connectivity---------------------
void Graph::traversalInitialize() {
	for(int i = 0; i < nVertices; i++) {
		vertices[i]->color = White;
		vertices[i]->key = INT_MAX;
		vertices[i]->mstParent = -1;
		mstCost = 0;
	}
}

void Graph::dfsVisit(int u) {
	vertices[u]->color = Black;
	vector<EdgeEnd> edgeEnds = vertices[u]->edges;
	for(int i = 0; i < edgeEnds.size(); i++) {
		int edgeEnd  = edgeEnds[i].index;
		if(vertices[edgeEnd]->color != Black)
			dfsVisit(edgeEnd);
	}
}

bool Graph::isConnected() {
	//Initialize for DFS search
	traversalInitialize();
	dfsVisit(0);
	for(int i = 0; i < nVertices; i++) {
		if(vertices[i]->color != Black)
			return false;
	}
	return true;
}

//-------------------------------------------Calculate MST--------------------------
int Graph::getWeight(int u, int v) {
	Vertex *U = vertices[u];
	for(int i = 0; i < U->edges.size(); i++) {
		if(U->edges[i].index == v)
			return U->edges[i].weight;
	}
	return INT_MAX;
}

void Graph::primMST(MinQueue *q) {
	traversalInitialize();
	vertices[0]->key = 0;
	while(!q->isEmpty()) {
		int u = q->extractMin();
		for(int i = 0; i < vertices[u]->edges.size(); i++) {
			int v = vertices[u]->edges[i].index;
			int w = getWeight(u, v);
			if(vertices[v]->color == White && w < vertices[v]->key){
				q->decreaseKey(v, w);
				vertices[v]->mstParent = u;
			}
		}
		mstCost += vertices[u]->key;
		vertices[u]->color = Black;
	}
}

void Graph::printMST() {
	// first check whether mst is calculated or not
	// no more two nodes can have mstParent to be -1
	if(mstCost < 0) { // does not support negative weight
		cout <<"! ! ! mst does not exist yet, please calculate first"<<endl;
		return;
	}
	// mst exists
	cout <<mstCost<<endl;
	for(int i = 1; i < nVertices; i++) {
		cout << vertices[i]->mstParent <<"\t" << i <<endl;
	}
}

Graph::~Graph() {
	// TODO Auto-generated destructor stub
}

