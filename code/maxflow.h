#ifndef MAXFLOW_H
#define MAXFLOW_H

#include "graph.h"
#include "list.h"
#include "edge.h"

/* Do breadth first on a graph and return pointer to list containing the
 * augmenting path
 * Args:    - Pointer to the graph
 * Return:  A pointer to list containing the augmenting path*/
struct list* bfs(struct graph*);

/* Do depth first search on a graph and return pointer
 * Args:    - Pointer to the graph
 * Return:  A pointer to list containing the augmenting path*/
struct list* dfs(struct graph* g);

/* Subrutine of depth first search that visits vertex u's adjacent vertices if
 * they have not been visited before and there is residual capacity on (u,v)
 * Args:    - Pointer to the graph
 *          - Vertex u
 *          - Array that tells which vertices have been visited
 *          - Array with edges that goes from vertices parents to vertex.
 * Return:  Nothing */
void dfs_visit(struct graph* g, int u, int visited[], struct edge* parents[]);

/* Find the minimum capacity of the augmenting path
 * Args:    - Pointer to augmenting path in list
 * Return:  The path capacity */
int get_cfp(struct list* augpath);

/* Get the residual capacity of edge
 * Args:    - Pointer to edge
 * Return:  Residual capacity of edge */
int get_cfe(struct edge*);

/* Augment path with its residual value cfp
 * Args:    - Pointer to augmenting path
 *          - The path's residual capacity
 * Return:  Nothing */
void augment(struct list* path, int cfp);

/* Get maximum flow on graph using search_algorithm to traverse graph
 * Args:    - Pointer to the graph
 *          - Function pointer to a graph search algorithm that takes a graph
 *          and returns a list pointer holding the path to augment
 * Return:  The maximum flow of graph g */
int get_maximum_flow(struct graph* g, struct list*(search_algo)(struct graph*));

/* Cast void* to int*, dereference and print it
 * Args:    - Void pointer to element
 * Return:  Nothing */
void print_int_elm(void*);

#endif

