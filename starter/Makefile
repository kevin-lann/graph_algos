mainprog: graph_tester.o minheap.o graph_algos.o graph.o
	gcc -g graph_tester.o minheap.o graph_algos.o graph.o -o mainprog

graph_tester.o: graph_tester.c minheap.c graph_algos.c graph.c
	gcc -g -c graph_tester.c

minheap.o: minheap.c minheap.h
	gcc -g -c minheap.c

graph_algos.o: graph_algos.c graph_algos.h minheap.c minheap.h graph.c graph.h
	gcc -g -c graph_algos.c 

graph.o: graph.c graph.h
	gcc -g -c graph.c

clean:
	rm -f *.o mainprog
.PHONY: clean