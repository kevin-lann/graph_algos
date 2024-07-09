/*
 * Our Priority Queue implementation.
 *
 * Author: Akshay Arun Bapat
 * Based on implementation from A. Tafliovich
 */

#include "minheap.h"

bool isValidIndex(MinHeap* heap, int nodeIndex);
int getLeftChildIdx(int nodeIndex);
int getRightChildIdx(int nodeIndex);
int getParentIdx(int nodeIndex);
int priorityAt(MinHeap* heap, int nodeIndex);
HeapNode nodeAt(MinHeap* heap, int nodeIndex);
int idAt(MinHeap* heap, int nodeIndex);
int indexOf(MinHeap* heap, int id);


#define ROOT_INDEX 1
#define NOTHING -1

/*************************************************************************
 ** Suggested helper functions -- part of starter code
 *************************************************************************/

/*
 * Swaps contents of heap->arr[index1] and heap->arr[index2] if both
 * 'index1' and 'index2' are valid indices for minheap 'heap'.
 * Has no effect otherwise.
 */
void swap(MinHeap* heap, int index1, int index2)
{
  if (!isValidIndex(heap, index1) || !isValidIndex(heap, index2))
    return;
  
  HeapNode* n1 = &heap->arr[index1];
  HeapNode* n2 = &heap->arr[index2];

  int tmp_id = n1->id;
  int tmp_pri = n1->priority;

  heap->indexMap[n1->id] = index2;
  heap->indexMap[n2->id] = index1;

  n1->id = idAt(heap, index2);
  n1->priority = priorityAt(heap, index2);

  n2->id = tmp_id;
  n2->priority = tmp_pri;
}

/*
 * Floats up the element at index 'nodeIndex' in minheap 'heap' such that
 * 'heap' is still a minheap.
 * Precondition: 'nodeIndex' is a valid index of minheap 'heap'
 */
void floatUp(MinHeap* heap, int nodeIndex)
{
  int parentIndex = getParentIdx(nodeIndex);
  while (parentIndex >= ROOT_INDEX 
    && priorityAt(heap, nodeIndex) < priorityAt(heap, parentIndex))
  {
    swap(heap, nodeIndex, parentIndex);
    nodeIndex = parentIndex;
    parentIndex = getParentIdx(parentIndex);
  }
}

/*
 * Returns the index of the left child of a node at index 'nodeIndex',
 * assuming it exists.
 * Precondition: 'nodeIndex' is a valid index of minheap 'heap'
 */
int getLeftChildIdx(int nodeIndex)
{
  return 2 * nodeIndex;
}

/*
 * Returns the index of the right child of a node at index 'nodeIndex',
 * assuming it exists.
 * Precondition: 'nodeIndex' is a valid index of minheap 'heap'
 */
int getRightChildIdx(int nodeIndex)
{
  return 2 * nodeIndex + 1;
}

/*
 * Returns the index of the parent of a node at index 'nodeIndex',
 * assuming it exists.
 * Precondition: 'nodeIndex' is a valid index of minheap 'heap'
 */
int getParentIdx(int nodeIndex)
{
  return nodeIndex / 2;
}

/*
 * Returns True if 'nodeIndex' is currently a valid index in minheap 'heap'.
 * False otherwise.
 */
bool isValidIndex(MinHeap* heap, int nodeIndex)
{
  return ROOT_INDEX <= nodeIndex && nodeIndex <= heap->size; 
}

/*
 * Returns priority of node at index 'nodeIndex' in minheap 'heap'.
 * Precondition: 'nodeIndex' is a valid index of minheap 'heap'
 *               'heap' is non-empty
 */
int priorityAt(MinHeap* heap, int nodeIndex)
{
  return heap->arr[nodeIndex].priority;
}

/*
 * Returns node at index 'nodeIndex' in minheap 'heap'.
 * Precondition: 'nodeIndex' is a valid index in 'heap'
 *               'heap' is non-empty
 */
HeapNode nodeAt(MinHeap* heap, int nodeIndex)
{
  return heap->arr[nodeIndex];
}

/*
 * Returns ID of node at index 'nodeIndex' in minheap 'heap'.
 * Precondition: 'nodeIndex' is a valid index in 'heap'
 *               'heap' is non-empty
 */
int idAt(MinHeap* heap, int nodeIndex)
{
  return heap->arr[nodeIndex].id;
}

/*
 * Returns index of node with ID 'id' in minheap 'heap'.
 * Precondition: 'id' is a valid ID in 'heap'
 *               'heap' is non-empty
 */
int indexOf(MinHeap* heap, int id)
{
  return heap->indexMap[id];
}

/*********************************************************************
 * Required functions
 ********************************************************************/
HeapNode getMin(MinHeap* heap)
{
  return heap->arr[ROOT_INDEX];
}

void heapify(MinHeap* heap, int nodeIndex)
{
  int leftIdx = getLeftChildIdx(nodeIndex);
  int rightIdx = getRightChildIdx(nodeIndex);

  if (!isValidIndex(heap, rightIdx) && !isValidIndex(heap, leftIdx))
    return;

  // go left
  if (priorityAt(heap, leftIdx) < priorityAt(heap, nodeIndex)
      && ( !isValidIndex(heap, rightIdx) || priorityAt(heap, leftIdx) <= priorityAt(heap, rightIdx))) 
  {
    swap(heap, nodeIndex, leftIdx);
    heapify(heap, leftIdx);
  }
  // go right
  else if (priorityAt(heap, rightIdx) < priorityAt(heap, nodeIndex)
          && priorityAt(heap, rightIdx) <= priorityAt(heap, leftIdx))
  {
    swap(heap, nodeIndex, rightIdx);
    heapify(heap, rightIdx);
  }
}

HeapNode extractMin(MinHeap* heap)
{
  HeapNode min = getMin(heap);

  // swap top with bottom rightmost
  swap(heap, ROOT_INDEX, heap->size);

  // "delete" bottom rightmost
  --heap->size;

  heapify(heap, 1);
  return min;
}

bool insert(MinHeap* heap, int priority, int id)
{
  if (heap->size + 1 > heap->capacity)
    return false;
  
  ++heap->size;
  // insert at bottom rightmost
  heap->arr[heap->size].priority = priority;
  heap->arr[heap->size].id = id;
  heap->indexMap[id] = heap->size;

  floatUp(heap, heap->size);
  return true;
}

int getPriority(MinHeap* heap, int id)
{
  return nodeAt(heap, indexOf(heap, id)).priority;
}

bool decreasePriority(MinHeap* heap, int id, int newPriority)
{
  int idx = indexOf(heap, id);

  if (!isValidIndex(heap, idx) || nodeAt(heap, idx).priority <= newPriority)
    return false;

  heap->arr[idx].priority = newPriority;
  floatUp(heap, idx);

  return true;
}

/*********************************************************************
 ** Helper function provided in the starter code
 *********************************************************************/
void printHeap(MinHeap* heap)
{
  printf("MinHeap with size: %d\n\tcapacity: %d\n\n", heap->size,
         heap->capacity);
  printf("index: priority [ID]\t ID: index\n");
  for (int i = 0; i < heap->capacity; i++)
    printf("%d: %d [%d]\t\t%d: %d\n", i, heap->arr[i].priority,
           heap->arr[i].id, i, heap->indexMap[i]);
  printf("\n\n");
}

/***** Memory management (sample solution) **********************************/
MinHeap* newHeap(int capacity)
{
  MinHeap* new = (MinHeap*) malloc(sizeof(MinHeap));
  new->arr = (HeapNode*) malloc((capacity+2) * sizeof(HeapNode));
  new->indexMap = (int*) malloc((capacity) * sizeof(int));
  new->capacity = capacity;
  new->size = 0;
  return new;
}

void deleteHeap(MinHeap* heap)
{
  free(heap->arr);
  free(heap->indexMap);
  free(heap);
}
