#ifndef QUEUE_H
#define QUEUE_H

#include "container.h"

/* A queue is an elemental first in-first out (FIFO) data structure, containing
 * a pointer to the head-/the first element added and a pointer to the element
 * last added to the queue */
struct queue {
    int size;
    struct element* head;
    struct element* last;
};

/* Initialize an empty queue with size 0
 * Return:  Pointer to empty queue */
struct queue* queue_init();

/* Add an element to the back of the queue
 * Args:    - Pointer to queue
 *          - A pointer to the element to be enqueued
 * Return:  Nothing */
void enqueue(struct queue*, void*);

/* Removes an element from the front of the queue
 * Args:    Pointer to queue
 * Return:  A pointer to dequed and unwrapped element. NULL if queue is empty.*/
void* dequeue(struct queue*);

/* Tells if the queue is empty
 * Args     - Pointer to queue
 * Return   1 if queue is empty, 0 if queue contains elements */
int queue_is_empty(struct queue*);

#endif

