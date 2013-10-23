/*
 * MinQueue.cpp
 *
 *  Created on: Oct 11, 2013
 *      Author: yahuihan
 */

#include "MinQueue.h"

// --------------------------------SimpleQueue------------------------------------
SimpleQueue::SimpleQueue(Graph *gh){
//	g = gh;
//	elements = g->nVertices;
	n = gh->nVertices;
	data = (Type *) malloc(sizeof(Type) * n);
	for(int i = 0; i < n; i++) {
		data[i] = gh->vertices[i];
	}
}

bool SimpleQueue::isEmpty() {
	for(int i = 0; i < n; i++ ) {
		if(data[i]->color == White)
			return false;
	}
	return true;
}

int SimpleQueue::extractMin() {
	int res = -1;
	int minKey = INT_MAX;
	for(int i = 0; i < n; i++ ) {
		if(data[i]->color == White && data[i]->key < minKey) {
			res = i;
			minKey = data[i]->key;
		}
	}
	return res;
}

void SimpleQueue::decreaseKey(int v, int value) {
	if(data[v]->key <= value) return;
	data[v]->key = value;
}

// --------------------------------Fibonacci Heap-------------------------------
FibonacciHeap::FibonacciHeap(Graph *gh) {
	graph = gh;
	heap = NULL;
	f_map = (fnode**) malloc(sizeof(fnode *) * gh->nVertices);
	initialize(gh);
}

FibonacciHeap::~FibonacciHeap() {
	if(heap)
		//free the memory of all the nodes
		deleteAll(heap);
	if(f_map)
		free(f_map);
}

bool FibonacciHeap::isEmpty() {
	return heap == NULL;
}

int FibonacciHeap::extractMin() {
	fnode* old = heap;
	heap = removeMin(heap);
	Type ret = old->data;
	delete old;
//	cout <<"Extract out "<<ret->id<<endl;
	return ret->id;
}

//Cascading cut
void FibonacciHeap::decreaseKey(int v, int value) {
//	cout <<"Decreasing " <<v <<" to value "<<value<<endl;
	if(graph->vertices[v]->key <= value)
		return;

	//Find the node in the f heap
	fnode *n = find(heap, v);

	n->data->key = value;

	//In case n has no parent and change the heap min pointer if necessary
	if(n->parent == NULL) {
		if(n->data->key < heap->data->key)
			heap = n;
		return;
	}

	// need to do the cascading cut
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
		fnode *n = insert(g->vertices[i]);
		f_map[i] = n;
	}
}

// create a node and insert it into the top level linked list
fnode *FibonacciHeap::insert(Type v) {
	fnode* n = new fnode(v);
	heap = merge(heap,n);
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

// Remove all the nodes in a node's list from their parent
void FibonacciHeap::removeAllFromParent(fnode *n){
	if(n == NULL) return;
	fnode* c = n;
	do {
		c->childCut = false;
		c->parent = NULL;
		c=c->right;
	}while(c != n);
}

// remove min and do the pairwise combine and return new min
fnode *FibonacciHeap::removeMin(fnode *n) {
	removeAllFromParent(n->child);
	if(n->right == n) {  // Only child
		n = n->child;
	} else {
		// remove this node from its linked list and reinsert its children back in the list
		n->right->left = n->left;
		n->left->right = n->right;
		n = merge(n->right,n->child);
	}

	if(n == NULL)
		return n;

	// pairwise combine
	fnode* trees[64]={NULL};

	while(true) {
		if(trees[n->degree]!=NULL) {  // there are node of same degree, combine
			fnode* t = trees[n->degree];
			if(t == n) // break the loop,
				break;
			trees[n->degree] = NULL;
			if(n->data->key < t->data->key) {
				t->left->right=t->right; // extract t from the lists
				t->right->left=t->left;
				addChild(n,t);
			} else {
				t->left->right=t->right;
				t->right->left=t->left;
				if(n->right == n) { // only child
					t->right = t->left=t;
					addChild(t,n);
					n = t;
				} else {
					// replace n with t in the list and add the n as the child of t
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
	child->parent = parent;
	parent->degree++;
	parent->child = merge(parent->child, child);
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
	n->childCut = false;
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

fnode* FibonacciHeap::find(fnode *heap, int id) {
//	unordered_map<int, fnode*>::iterator it = f_map->find(id);
//	if(it != f_map->end())
//		return it->second;
	if(id >= 0 && id < graph->nVertices)
		return f_map[id];
	return NULL;
}
