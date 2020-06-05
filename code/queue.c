#include "queue.h"
#include "list.h"
#include "edge.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

struct queue* queue_init() {
    struct queue* q = malloc(sizeof(struct queue));
    assert(q != NULL);
    q->head  = NULL;
    q->last  = NULL;
    q->size  = 0;
    return q;
}

int queue_is_empty(struct queue* q) {
    return !q->size;
}

void enqueue(struct queue* q, void* new_elm) {
    struct element* elm = malloc(sizeof(struct element));
    assert(elm != NULL);

    elm->this = new_elm;
    elm->next = NULL;

    if (queue_is_empty(q)) {
        q->head = elm;
    } else { // If the queue aren't empty the last element's next is 'elm'
        q->last->next = elm;
    }
    q->last = elm;
    ++q->size;
}

void* dequeue (struct queue* q) {
    if (!q->size) {
        return NULL;
    }
    struct element* elm = q->head;
    void* ret = elm->this; // Unwrap element
    q->head = elm->next;
    free(elm);
    --q->size;
    return ret;
}

