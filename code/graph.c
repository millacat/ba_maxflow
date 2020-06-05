#include "graph.h"
#include "container.h"
#include "list.h"
#include "edge.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

struct graph* graph_init(int size) {
    assert(0 < size);

    struct graph* g = malloc(sizeof(struct graph));
    assert(g != NULL);

    g->size = size;
    g->vertices  = malloc(sizeof(int) * size);
    //g->parents   = malloc(sizeof(struct edge) * size - 1);
    g->adj_array = malloc(sizeof(struct list*) * size);
    assert(g->vertices  != NULL);
    //assert(g->parents   != NULL);
    assert(g->adj_array != NULL);

    for (int i = 0; i < size; i++) {
        g->vertices[i] = i;
        //g->parents[i] = NULL;
        g->adj_array[i] = list_init(); // List pointer in each entry
    }

    return g;
}

void graph_free(struct graph* g) {
    assert(g != NULL);

    for (int i = 0; i < g->size; i++) {
        // Both backward and forward edges are placed in the adjlists, hence it
        // must not free a backward edge when encountering a forward edge, then
        // it cannot free it when it encounters it later in the adjacency lists
        container_free(g->adj_array[i], g->adj_array[i]->head, 1, 0);
    }

//    free(g->parents);
    free(g->vertices);
    free(g->adj_array);
    free(g);
}

void graph_add_edges(struct graph* g, int from, int to, int capacity) {
    assert(g != NULL);
    assert(from != to);
    assert(0 <= from);
    assert(0 <= to);
    assert(0 <= capacity);
    assert(from < g->size);
    assert(to   < g->size);

    struct edge* e = edges_init(from, to, capacity);

    // Use list function to add the edges to the correct adjacency lists in the
    // adjacency array.
    // - Forward edge: (u, v) = (from, to). 'to' is kept in e->to.
    int success = list_insert_first(g->adj_array[from], e);
    assert(success == 0);
    // - Reverse edge: (v, u)
    success = list_insert_first(g->adj_array[to], e->reverse_edge);
    assert(success == 0);
}

void graph_print(struct graph* g) {
    // Iterating through adjacency array
    for (int i = 0; i < g->size; i++) {
        // Passing the adjacency list and a function pointer as argument
        container_print(g->adj_array[i]->head, &edge_print);
    }
}

