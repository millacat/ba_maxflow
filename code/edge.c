#include "edge.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

struct edge* edges_init(int from, int to, int capacity) {
    struct edge* e = malloc(sizeof(struct edge));
    assert(e != NULL);

    e->from     = from;
    e->to       = to;
    e->capacity = capacity;
    e->flow     = 0;
    e->original = 1;

    struct edge* rev = malloc(sizeof(struct edge));
    assert(rev != NULL);
    e->reverse_edge = rev;

    rev->from     = to;
    rev->to       = from;
    rev->capacity = 0;
    rev->flow     = 0;
    rev->original = 0;
    rev->reverse_edge = e;
    return e;
}

void edge_print(void* inp) {
    struct edge* e = (struct edge*) inp; // cast the void* to struct edge*
    if (e) {
        printf("from: %d, to: %d, c: %d, f: %d\n",
                e->from, e->to, e->capacity, e->flow);
    } else {
        printf("Edge is null.\n");
    }
}

void edge_free(struct edge* e) {
    free(e->reverse_edge);
    free(e);
}

