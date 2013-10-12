PrimMST
=======
@author: Yahui Han 
@email: hanyahui808@gmail.com

ADS project, fall 2013. An implementation of Prim's algorithm to calculate the minimum spanning tree in a connected graph.

It includes:

Graph.h: an implementation of graph using adjcent linked list

MinQueue.h: an interface which provide "extractMin", "decreaseKey", "isEmpty" and two implementations, including 
a simple array and a fibonacci heap

main.cpp: put them together and compare the performance of the two priority queue implementations.
