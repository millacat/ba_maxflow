#ifndef LIST_H
#define LIST_H

#include "container.h"

/* An implementation of a singly linked list. It contains a pointer to the head
 * element, which contains a pointer to its content and to the next element in
 * the list */

/* Making 'list' identifier available in the public namespace - Dunno if I want
 * to use this, but it is nice to do once to know about it :) */
typedef struct list list;

struct list {
    int size;
    struct element* head; // pointer to head element of list
};

/* Initialize an empty list
 * Return   Pointer to the empty list */
list* list_init();

/* Insert an element at front of the list
 * Args     - Pointer to the list
 *          - Pointer to element to insert
 * Return   0 on success, 1 on failure */
int list_insert_first(list* l, void* elm);

/* Insert element at end of the list
 * Args     - Pointer to the list
 *          - Pointer to element to insert
 * Return   0 on success, 1 on failure */
int list_insert_last(list* l, void* elm);

/* Remove the element at specific index
 * Args     - Pointer to the list
 *          - Index at which element is removed
 * Return   A pointer to the unwrapped element */
void* list_remove_element(list* l, int index);

/* Move element at index to front of list
 * Args     - Pointer to the list
 *          - Index at which element is moved from
 * Return   Nothing */
void list_move_to_front(list* l, int index);

#endif

