#ifndef PUSHRELABEL_H
#define PUSHRELABEL_H
#include "edge.h"
#include "list.h"
#include "graph.h"

/* Checks if push applies:
 * e(u) > 0, c_f(u,v) > 0, u.h = v.h + 1
 * Args:    - A pointer to an edge
 *          - Array containing vertices' heights
 * Return:  1 for true and 0 for false */
int push_applies(struct edge* uv, int* heights);

/* Push flow on edge
 * Args:    - A pointer to an edge
 *          - Array containing vertices' excess flow
 * Return:  Nothing */
void push(struct edge*, int* excess);

/* Checks if the relabel applies
 * Args:    - The vertex, u
 *          - Array with all vertices' excess
 *          - u's neighbor list
 *          - Array with all vertices' heights
 * Return:  1 for true and 0 for false */
int relabel_applies(int u, int* excess, struct list* neighbors, int* heights);

/* Relabels u : u.h = 1 + min{v.h : (u,v) in E_f}
 * Args:    - The vertex, u
 *          - u's neighbor list
 *          - Array with all vertices' heights
 * Return:  Nothing */
void relabel(int u, struct list* neighbors, int* heights);

/* Initialize preflow in G by pushing flow
 * on all edges going out from the source
 * Args:    - Pointer to graph
 *          - Array with all vertices' excess
 *          - Array with all vertices' heights
 * Return: Nothing */
void initialize_preflow(struct graph*, int* excess, int* height);

/* The Relabel-to-Front algorithm to use
 * on a graph to find the maximum flow
 * Args:    - Pointer to graph
 * Return:  The maximum flow
 * */
int relabel_to_front(struct graph*);

#endif

