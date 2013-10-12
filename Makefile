CC = g++ -O2 -Wno-deprecated

mst: main.o MinQueue.o Graph.o 
	$(CC) -o mst main.o MinQueue.o Graph.o 
	
main.o: main.cpp
	$(CC) -g -c main.cpp
	
Graph.o: Graph.cpp
	$(CC) -g -c Graph.cpp
	
MinQueue.o: MinQueue.cpp
	$(CC) -g -c MinQueue.cpp
	
clean: 
	rm -f *.o