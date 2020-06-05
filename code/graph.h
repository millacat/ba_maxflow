#ifndef GRAPH_H
#define GRAPH_H

#include "edge.h"

/* A graph is an array of adjacency lists */
// The source, s = 0
// The sink,   t = size - 1
// 'parents' is for running searches on the graph and finding vertices' parents
// 'vertices' is to be able to queue pointers to integers when running maxflow
// algorithms on the graph
struct graph {
    int size;
    int* vertices;
//    struct edge** parents;
    struct list** adj_array;
};

/* Initializes a graph with n = |V| = 'size' vertices
 * Args:    - The number of vertices in the graph
 * Return:  A pointer to the graph */
struct graph* graph_init(int size);

/* Add a forward- and backward edge to the graph
 * Args:    - Pointer to the graph
 *          - From vertex
 *          - To vertex
 *          - Capacity of the edge
 * Return:  Nothing */
void graph_add_edges(struct graph* g, int from, int to, int c);

/* Prints all the elements in the graph one entry in the adjacency array at the
 * time - one adjacency list at the time
 * Args:    - Pointer to the graph
 * Return:  Nothing */
void graph_print(struct graph*);

/* Frees the whole graph
 * Args:    - A pointer to the graph
 * Return:  Nothing */
void graph_free(struct graph*);

#endif
