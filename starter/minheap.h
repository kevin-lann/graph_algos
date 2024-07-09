/*
 * Header file for our Priority Queue implementation.
 *
 * You will NOT be submitting this file. Your code will be tested with
 * our own version of this file, so make sure you do not modify it!
 *
 * Author: Akshay Arun Bapat
 * Based on implementation from A. Tafliovich and F. Estrada
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef __MinHeap_header
#define __MinHeap_header

#define ROOT_INDEX 1
#define NOTHING -1

typedef struct heap_node {
  int priority;  // priority of this node
  int id;        // the unique ID of this node (vertex ID); (0 <= id < size
} HeapNode;

typedef struct min_heap {
  int size;       // the number of nodes in this heap; 0 <= size <= capacity
  int capacity;   // the number of nodes that can be stored in this heap
  HeapNode* arr;  // the array that stores the nodes of this heap
  int* indexMap;  // indexMap[id] is the index of node with ID id in array arr
} MinHeap;

/*
 * If 'nodeIndex' is the only node where heap property is not satisfied
 * in the minheap rooted at 'nodeIndex', then
 * Perform operations such that 'heap' becomes a valid minheap.
 *
 * Note: This is equivalent to floatUp but in the opposite direction.
 * Precondition: 'nodeIndex' is a valid index in 'heap'
 *               child subtrees of 'nodeIndex' are valid minheaps
 */
void heapify(MinHeap* heap, int nodeIndex);

/*
 * Returns the node with minimum priority in minheap 'heap'.
 * Precondition: heap is non-empty
 */
HeapNode getMin(MinHeap* heap);

/*
 * Removes and returns the node with minimum priority in minheap 'heap'.
 * Precondition: heap is non-empty
 */
HeapNode extractMin(MinHeap* heap);

/*
 * Inserts a new node with priority 'priority' and ID 'id' into minheap 'heap'.
 * Returns: true if insert was successful, false otherwise
 * Precondition: 'id' is unique within this minheap
 */
bool insert(MinHeap* heap, int priority, int id);

/*
 * Returns priority of the node with ID 'id' in 'heap'.
 * Precondition: 'id' is a valid node ID in 'heap'.
 */
int getPriority(MinHeap* heap, int id);

/*
 * Sets priority of node with ID 'id' in minheap 'heap' to 'newPriority', if
 * such a node exists in 'heap' and its priority is larger than
 * 'newPriority', and returns True. Has no effect and returns False, otherwise.
 */
bool decreasePriority(MinHeap* heap, int id, int newPriority);

/*
 * Prints the contents of this heap, including size and capacity. For
 * each non-empty element of the heap array, that node's ID and priority.
 */
void printHeap(MinHeap* heap);

/*
 * Returns a newly created empty minheap with initial capacity 'capacity'.
 * Precondition: capacity >= 0
 */
MinHeap* newHeap(int capacity);

/*
 * Frees all memory allocated for minheap 'heap'.
 */
void deleteHeap(MinHeap* heap);

#endif
