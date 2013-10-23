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

Graph::~Graph() {
	if(vertices)
		delete [] vertices;
}
//---------------------------------------Build a Graph----------------------------------
void Graph::buildFull (int n){
	initialize(n);
	int i, j, w;
	srand(time(NULL));

	for(i = 0; i < nVertices; i++) {
		for(j = i+1; j < nVertices; j++) {
			w = rand() % 1000 + 1;
			vertices[i]->edges[j] = w;
			vertices[j]->edges[i] = w;
		}
	}
//	cout <<"Build a full graph!"<<endl;
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
		w = rand() % 1000 + 1;
		if(addEdge(i, j, w)) // (i, j) not already in there
			e++;
	}
	while(! isConnected()) {
		i = rand() % n;
		j = rand() % n;
		w = rand() % 1000 + 1;
		if(addEdge(i, j, w))
			e++;
	}
//	cout <<"Actual density is : " << (double) 2 * e/(n * (n-1)) <<endl;
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
	if(i == j) return false;
	Vertex *v = vertices[i];
	// check edge (i, j) is in the graph or not
	unordered_map<int, int>::iterator eit = v->edges.find(j);
	if(eit != v->edges.end())
		return false;

	// e(i, j) not in the graph, then add
	v->edges[j] = w;
	vertices[j]->edges[i] = w;
	return true;
}

void Graph::initialize(int n) {
	this->nVertices  = n;
	this->vertices = (Vertex * *) malloc( n * sizeof(Vertex*));
	for(int i=0; i<n; i++)
		vertices[i] = new Vertex(i);
}

void Graph::print() {
	unordered_map<int, int>::iterator eit;
	for(int i = 0; i < nVertices; i++) {
		cout << i <<"--->";
		Vertex *v = vertices[i];
		for(eit = v->edges.begin(); eit != v->edges.end(); eit++)
			cout <<"  " <<eit->first <<"("<<eit->second <<")";
		cout <<endl;
	}
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
	unordered_map<int, int>::iterator eit;
	Vertex *v = vertices[u];
	for(eit = v->edges.begin(); eit != v->edges.end(); eit++) {
		int edgeEnd  = eit->first;
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
	Vertex *uV = vertices[u];
	unordered_map<int, int>::iterator eit = uV->edges.find(v);
	if(eit == uV->edges.end())
		return INT_MAX;
	return eit->second;
}

void Graph::primMST(MinQueue *q) {
	traversalInitialize();
	vertices[0]->key = 0;
	while(!q->isEmpty()) {
		int u = q->extractMin();
		unordered_map<int, int>::iterator eit;
		Vertex *uV = vertices[u];
		for(eit = uV->edges.begin(); eit != uV->edges.end(); eit++) {
			int v = eit->first;
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
		cout << vertices[i]->mstParent <<"\t" << i <<"\t"<<getWeight(i, vertices[i]->mstParent)<<endl;
	}
}

