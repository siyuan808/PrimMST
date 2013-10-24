#include <iostream>
#include <fstream>
#include "Graph.h"
#include "MinQueue.h"
#include <vector>
#include <time.h>
#include <string.h>

//#define VERBOSE
using namespace std;

void mstSimple(Graph *g) {
	if(! g->isConnected()) {
		cout <<"The graph is not connected, so MST does not exist"<<endl;
		return ;
	}
	// Calculating the mst on g
	SimpleQueue *q = new SimpleQueue(g);
	g->primMST(q);
//	g->printMST();
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
	//Performance comparison
	clock_t t, s;
	//-------------------simple queue-----------
	cout <<"------------------Simple Array------------------"<<endl;
	t = clock();
	mstSimple(g);
	t = clock() - t;
	cout <<"mst cost using simple array: " <<g->mstCost<<endl;
	printf("It took simple array %d microseconds!\n",(int)t);

	//-------------------Fibonacci heap-----------
	cout <<"------------------Fibonacci Heap----------------"<<endl;
	s = clock();
	mstFibonacci(g);
	s = clock() - s;
	cout <<"mst cost using Fibonacci heap: " <<g->mstCost<<endl;
	printf("It took fibonacci %d microseconds!\n",(int)s);
}

int main(int argc, char *args[]) {
	Graph *g = new Graph();
	if(argc < 2) {
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
		if(*c != '\0' || m > 100) {
			cout <<"Not valid arguments" <<endl;
			return 0;
		}
		clock_t start = clock();
		g->build(n, (double)m/100);
//		g->print();
//		return 0;
		randomMode(g);
		cout <<"Total cost is : "<<clock() - start<<endl;
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
	}
	//---------------------Do the comparison---------------
	else if(strcmp(args[1], "-t") == 0) {
		int nNodes[3] = {1000, 3000, 5000};
		double densities[10] = { 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
		int nTest = 5;
		time_t start, elapse, avg;
		ofstream out;
		out.open("output.xls");
		out <<"MST: Comparison of MinQueue between Array and Fibocacci Heap\n";
		for(int n = 0; n < 3; n++) {
			out <<nNodes[n]<<"\tArray\tfHeap\n";
			for(int d = 0; d < 10; d++) {
				cout <<"n: " <<nNodes[n] <<" d: " <<densities[d]<<endl;
				out <<densities[d]<<"\t";
				Graph *g = new Graph();
				g->build(nNodes[n], densities[d]);
				//Array
				avg = 0;
				for(int i=0; i < nTest; i++) {
					start = clock();
					mstSimple(g);
					elapse = clock()-start;
					avg += elapse;
#ifdef VERBOSE
					out <<elapse<<"\t";
#endif
				}
				out << avg/nTest<< "\t";
				//Fibonacci
				avg = 0;
				for(int i=0; i < nTest; i++) {
					start = clock();
					mstFibonacci(g);
					elapse = clock()-start;
					avg += elapse;
#ifdef VERBOSE
					out <<elapse<<"\t";
#endif
				}
				out << avg/nTest<< "\n";
				delete g;
			}
		}
		out.close();
	} else
		cout <<"Invalid mode, choose between -r, -f, -s, -t"<<endl;
	return 0;
}
