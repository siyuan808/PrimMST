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
	if(g->vertices[v]->key < value) return;
	g->vertices[v]->key = value;
}

// --------------------------------Fibonacci Heap-------------------------------
FibonacciHeap::FibonacciHeap(Graph *gh) {
	graph = gh;
	heap = NULL;
	initialize(gh);
}

FibonacciHeap::~FibonacciHeap() {
	if(heap)
		deleteAll(heap);
}

bool FibonacciHeap::isEmpty() {
	return heap == NULL;
}

int FibonacciHeap::extractMin() {
	fnode* old = heap;
	heap = removeMin(heap);
	Vertex* ret = old->data;
	delete old;
//	cout <<"Extract out "<<ret->id<<endl;
	return ret->id;
}

//Cascading cut
void FibonacciHeap::decreaseKey(int v, int value) {
//	cout <<"Decreasing " <<v <<" to value "<<value<<endl;
	fnode *n = find(heap, graph->vertices[v]);

	if(n->data->key < value)
		return;

	n->data->key = value;

	//In case n has no parent
	if(n->parent == NULL) {
		if(n->data->key < heap->data->key)
			heap = n;
		return;
	}

	if(n->data->key < n->parent->data->key) {
		heap = cut(heap,n);
		fnode* parent = n->parent;
		n->parent = NULL;
		while(parent != NULL && parent->childCut) {
			heap = cut(heap,parent);
			n = parent;
			parent = n->parent;
			n->parent = NULL;
		}
		if(parent!=NULL && parent->parent!=NULL)
			parent->childCut = true;
	}
}

//--------------------Fibonacci Heap Private---------------------
void FibonacciHeap::initialize(Graph *g){
	for(int i = 0; i < g->nVertices; i++) {
		insert(g->vertices[i]);
	}
}

// create a node and insert it into the top level linked list
fnode *FibonacciHeap::insert(Vertex* v) {
	fnode* n = singleton(v);
	heap = merge(heap,n);
	return n;
}

//return a single node min tree
fnode *FibonacciHeap::singleton(Vertex* value) {
	fnode* n=new fnode;
	n->data=value;
	n->left=n->right=n;
	n->degree=0;
	n->childCut=false;
	n->child=NULL;
	n->parent=NULL;
	return n;
}


// merge node b into a's linked list
fnode *FibonacciHeap::merge(fnode *a, fnode *b){
	if(a==NULL) return b;
	if(b==NULL) return a;
	if(a->data->key > b->data->key) {
		fnode* temp=a;
		a=b;
		b=temp;
	}
	fnode* an=a->right;
	fnode* bp=b->left;
	a->right=b;
	b->left=a;
	an->left=bp;
	bp->right=an;
	return a;
}

void FibonacciHeap::unMarkAndunParentAll(fnode *n){
	if(n == NULL)return;
	fnode* c=n;
	do {
		c->childCut = false;
		c->parent = NULL;
		c=c->right;
	}while(c!=n);
}

// remove min and do the pairwise combine
fnode *FibonacciHeap::removeMin(fnode *n) {
	unMarkAndunParentAll(n->child);
	if(n->right == n) {  // Only child
		n = n->child;
	} else { // remove this node
		n->right->left = n->left;
		n->left->right = n->right;
		n = merge(n->right,n->child);
	}
	if(n==NULL)return n;
	fnode* trees[64]={NULL};

	while(true) {
		if(trees[n->degree]!=NULL) {  // there are node of same degree, combine
			fnode* t = trees[n->degree];
			if(t == n)
				break;
			trees[n->degree] = NULL;
			if(n->data->key < t->data->key) {
				t->left->right=t->right;
				t->right->left=t->left;
				addChild(n,t);
			} else {
				t->left->right=t->right;
				t->right->left=t->left;
				if(n->right==n) {
					t->right=t->left=t;
					addChild(t,n);
					n = t;
				} else {
					n->left->right=t;
					n->right->left=t;
					t->right=n->right;
					t->left=n->left;
					addChild(t,n);
					n = t;
				}
			}
			trees[n->degree] = n; // add the new tree in track
			continue;
		} else {
			trees[n->degree] = n;
		}
		n=n->right;
	}
	fnode* min = n;
	fnode *ptr = n;
	do {
		if(ptr->data->key < min->data->key)
			min = ptr;
		ptr = ptr->right;
	} while(ptr != n);
	return min;
}

void FibonacciHeap::addChild(fnode *parent, fnode *child){
	child->right = child->left = child;
	child->parent=parent;
	parent->degree++;
	parent->child = merge(parent->child,child);
}

fnode *FibonacciHeap::cut(fnode *heap, fnode *n){
	if(n->right == n) {
		n->parent->child = NULL;
	} else {
		n->right->left = n->left;
		n->left->right = n->right;
		n->parent->child = n->right;
	}
	n->right = n->left = n;
	n->childCut=false;
	return merge(heap,n);
}

void FibonacciHeap::deleteAll(fnode *n) {
	if(n!=NULL) {
		fnode* c=n;
		do {
			fnode* d=c;
			c=c->right;
			deleteAll(d->child);
			delete d;
		} while(c!=n);
	}
}

fnode* FibonacciHeap::find(fnode *heap, Vertex* value) {
	fnode* n = heap;
	if(n == NULL) return NULL;
	do {
		if(n->data == value) return n;
		fnode* ret = find(n->child,value);
		if(ret)
			return ret;
		n = n->right;
	}while(n!=heap);
	return NULL;
}
