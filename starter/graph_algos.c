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
  int* distances;     // distances[id] is distance(start, id) in Dijkstra's.
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
  records->distances = (int *) malloc (graph->numVertices * sizeof (int));
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

  for (int id = 0; id < graph->numVertices; id++)
    if (id != startVertex)
      insert (min_heap, INT_MAX, id);
  
  return min_heap; 
}

/*
 * Returns true iff 'heap' is NULL or is empty.
 */
bool isEmpty(MinHeap* heap)
{
  return heap == NULL || heap->size == 0;
}

/*
 * Prints the status of all current algorithm data: good for debugging.
 */
void printRecords(Records* records);

/*
 * Add a new edge to records at index ind.
 */
void addTreeEdge(Records* records, int ind, int fromVertex, int toVertex,
                 int weight)
{
  Edge *edge = newEdge (fromVertex, toVertex, weight);

  records->tree[ind] = *edge;
  records->numTreeEdges++;
}

/*
 * Creates and returns a path from 'vertex' to 'startVertex' from edges
 * in the distance tree 'distTree'.
 */
EdgeList* makePath(Edge* distTree, int vertex, int startVertex);

/* Returns true iff id is a valid id in the graph 'graph'. */
bool isValidNode(Graph* graph, int id)
{
  return 0 <= id && id < graph->numVertices;
}

/*************************************************************************
 ** Required functions
 *************************************************************************/

Edge* getMSTprim(Graph* graph, int startVertex)
{
  if (!isValidNode (graph, startVertex))
    return NULL;

  Records* rec = initRecords(graph, startVertex);

  while (!isEmpty (rec->heap))
  {
    HeapNode u = extractMin (rec->heap);
    rec->finished[u.id] = true;

    /* Omit adding the start node, because it doesn't have a predecessor. */
    if (u.id != startVertex)
      addTreeEdge (rec, rec->numTreeEdges, u.id, rec->predecessors[u.id], u.priority);

    /* Iterate through u's adjacency list. */
    EdgeList* l = graph->vertices[u.id]->adjList;
    while (l != NULL)
    {
      Vertex* v = graph->vertices[l->edge->toVertex];
      /* If v in heap and w(u, v) less than priority(v) . */
      if (rec->finished[v->id] == false && l->edge->weight < getPriority (rec->heap, v->id))
      {
        decreasePriority(rec->heap, v->id, l->edge->weight);
        rec->predecessors[v->id] = u.id; 
      }
      l = l->next;
    }
  }

 return rec->tree;
}

Edge* getDistanceTreeDijkstra(Graph* graph, int startVertex)
{
  if (!isValidNode (graph, startVertex))
    return NULL;

  Records* rec = initRecords(graph, startVertex);
  rec->distances[startVertex] = 0;

  while (!isEmpty (rec->heap))
  {
    HeapNode u = extractMin (rec->heap);
    rec->finished[u.id] = true;
    
    /* Iterate through u's adjacency list. */
    EdgeList* l = graph->vertices[u.id]->adjList;
    while (l != NULL)
    {
      Vertex* v = graph->vertices[l->edge->toVertex];
      int new_dist = getPriority (rec->heap, u.id) + l->edge->weight;
      /* If v in heap and dist(start, v) less than priority(v) . */
      if (rec->finished[v->id] == false && new_dist < getPriority(rec->heap, v->id))
      {
        decreasePriority(rec->heap, v->id, new_dist);
        rec->distances[v->id] = new_dist;
        rec->predecessors[v->id] = u.id; 
      }
      l = l->next;
    }
  }

  /* Build Distance Tree */
  addTreeEdge (rec, rec->numTreeEdges, startVertex, startVertex, 0);
  for (int id = 0; id < graph->numVertices; id++)
    if (id != startVertex)
      addTreeEdge (rec, rec->numTreeEdges, id, rec->predecessors[id], rec->distances[id]);

  return rec->tree;
}

EdgeList** getShortestPaths(Edge* distTree, int numVertices, int startVertex)
{
  if (!(0 <= startVertex && startVertex < numVertices))
    return NULL;
  
  EdgeList **paths = (EdgeList **) malloc (numVertices * sizeof (EdgeList *));
  paths[startVertex] = NULL;

  for (int id = 0; id < numVertices; id++)
  {
    if (id == startVertex)
      continue;

    Edge *edge = &distTree[id];
    EdgeList *list, *start, *prev;
    prev = newEdgeList (NULL, NULL);

    /* Add the path to list. Path is unique and terminates at startVertex. */
    while (edge->fromVertex != startVertex)
    {
      list = newEdgeList (&distTree[edge->fromVertex], NULL);
      prev->next = list;
      
      /* Record start node of list. */
      if (edge->fromVertex == id)
        start = prev->next;

      list = list->next;
      prev = prev->next;

      edge = &distTree[edge->toVertex];
    }

    paths[id] = start;
    free (prev);
  }

  return paths;
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
