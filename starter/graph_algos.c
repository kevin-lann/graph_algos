/*
 * Graph algorithms.
 *
 * Author: Akshay Arun Bapat
 * Based on implementation from A. Tafliovich
 */

#include <limits.h>

#include "graph.h"
#include "minheap.h"

MinHeap* initHeap(Graph* graph, int startVertex);

/*
 * A structure to keep record of the current running algorithm.
 */
typedef struct records
{
  int numVertices;    // total number of vertices in the graph
                      // vertex IDs are 0, 1, ..., numVertices-1
  MinHeap* heap;      // priority queue
  bool* finished;     // finished[id] is true iff vertex id is finished
                      //   i.e. no longer in the PQ
  int* predecessors;  // predecessors[id] is the predecessor of vertex id
  Edge* tree;         // keeps edges for the resulting tree
  int numTreeEdges;   // current number of edges in mst
} Records;

/*************************************************************************
 ** Suggested helper functions -- part of starter code
 *************************************************************************/

/*
 * Creates, populates, and returns all records needed to run Prim's and
 * Dijkstra's algorithms on Graph 'graph' starting from vertex with ID
 * 'startVertex'.
 * Precondition: 'startVertex' is valid in 'graph'
 */
Records* initRecords(Graph* graph, int startVertex)
{
  Records *records = (Records *) malloc (sizeof (Records));

  records->numVertices = graph->numVertices;
  records->heap = initHeap(graph, startVertex);

  // allocates and initializes all entries to false.
  records->finished = (bool *) calloc (graph->numVertices, sizeof (bool));

  records->predecessors = (int *) malloc (graph->numVertices * sizeof (int));
  records->tree = (Edge *) malloc (graph->numVertices * sizeof (Edge));
  records->numTreeEdges = 0;

  return records;
}

/*
 * Creates, populates, and returns a MinHeap to be used by Prim's and
 * Dijkstra's algorithms on Graph 'graph' starting from vertex with ID
 * 'startVertex'.
 * Precondition: 'startVertex' is valid in 'graph'
 */
MinHeap* initHeap(Graph* graph, int startVertex)
{
  MinHeap *min_heap = newHeap (graph->numVertices);

  insert (min_heap, 0, startVertex);

  for (int i = 0; i < graph->numVertices; i++)
    if (graph->vertices[i]->id != startVertex)
      insert (min_heap, INT_MAX, i);
  
  return min_heap; 
}

/*
 * Returns true iff 'heap' is NULL or is empty.
 */
bool isEmpty(MinHeap* heap);

/*
 * Prints the status of all current algorithm data: good for debugging.
 */
void printRecords(Records* records);

/*
 * Add a new edge to records at index ind.
 */
void addTreeEdge(Records* records, int ind, int fromVertex, int toVertex,
                 int weight);

/*
 * Creates and returns a path from 'vertex' to 'startVertex' from edges
 * in the distance tree 'distTree'.
 */
EdgeList* makePath(Edge* distTree, int vertex, int startVertex);

/*************************************************************************
 ** Required functions
 *************************************************************************/

Edge* getMSTprim(Graph* graph, int startVertex)
{
 return NULL;
}

Edge* getDistanceTreeDijkstra(Graph* graph, int startVertex)
{
  return NULL;
}

EdgeList** getShortestPaths(Edge* distTree, int numVertices, int startVertex)
{
  return NULL;
}

/*************************************************************************
 ** Provided helper functions -- part of starter code to help you debug!
 *************************************************************************/
void printRecords(Records* records)
{
  if (records == NULL)
    return;

  int numVertices = records->numVertices;
  printf("Reporting on algorithm's records on %d vertices...\n", numVertices);

  printf("The PQ is:\n");
  printHeap(records->heap);

  printf("The finished array is:\n");
  for (int i = 0; i < numVertices; i++)
    printf("\t%d: %d\n", i, records->finished[i]);

  printf("The predecessors array is:\n");
  for (int i = 0; i < numVertices; i++)
    printf("\t%d: %d\n", i, records->predecessors[i]);

  printf("The TREE edges are:\n");
  for (int i = 0; i < records->numTreeEdges; i++) printEdge(&records->tree[i]);

  printf("... done.\n");
}
