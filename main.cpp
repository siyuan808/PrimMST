#include <iostream>
#include "Graph.h"
#include "MinQueue.h"
#include <vector>
#include <time.h>

using namespace std;

void mstSimple(Graph *g) {
	if(! g->isConnected()) {
		cout <<"The graph is not connected, so MST does not exist"<<endl;
		return ;
	}
	// Calculating the mst on g
	SimpleQueue *q = new SimpleQueue(g);
	g->primMST(q);
	delete q;
}

void mstFibonacci(Graph *g) {
	if(! g->isConnected()) {
		cout <<"The graph is not connected, so MST does not exist"<<endl;
		return ;
	}
	// Calculating the mst on g
	FibonacciHeap *f = new FibonacciHeap(g);
	g->primMST(f);
	delete f;
}

void randomMode(Graph *g) {
	clock_t t;
	//-------------------simple queue-----------
	cout <<"------------------Simple Array------------------"<<endl;
	t = clock();
	mstSimple(g);
	t = clock() - t;
	cout <<"mst cost using simple array: " <<g->mstCost<<endl;
	printf("It took simple array %d microseconds!\n",(int)t);

	//-------------------Fibonacci heap-----------
	cout <<"------------------Fibonacci Heap----------------"<<endl;
	t = clock();
	mstFibonacci(g);
	t = clock() - t;
	cout <<"mst cost using Fibonacci heap: " <<g->mstCost<<endl;
	printf("It took fibonacci %d microseconds!\n",(int)t);
}

int main(int argc, char *args[]) {
	Graph *g = new Graph();
	if(argc < 3) {
		cout <<"Too few arguments" <<endl;
		return 0;
	}
	//--------------------Random Mode---------------------
	if(strcmp(args[1], "-r") == 0) {
		if(argc < 4) {
			cout <<"Too few arguments" <<endl;
			return 0;
		}
		int n = atoi(args[2]);
		int m = 0;
		char *c = args[3];
		while(*c != '\0' && *c >= '0' && *c <= '9') {
			m = m * 10 + (*c - '0');
			c++;
		}
		if(*c != '%' || m > 100) {
			cout <<"Not valid arguments" <<endl;
			return 0;
		}
		g->build(n, (double)m/100);
//		g->print();
		randomMode(g);
	}
	//---------------------Simple Mode--------------------
	else if(strcmp(args[1], "-s") == 0) {
		g->build(args[2]);
		mstSimple(g);
		g->printMST();
	}
	//---------------------f Mode--------------------------
	else if(strcmp(args[1], "-f") == 0) {
		g->build(args[2]);
		mstFibonacci(g);
		g->printMST();
	} else
		cout <<"Invalid mode, choose between -r, -f, -s"<<endl;
	return 0;
}
