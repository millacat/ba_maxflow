#ifndef EDGE_H
#define EDGE_H

/* An edge contains information abut which nodes it goes from and to,
 * (u,v) = (from, to),
 * and about its capacity and current flow. It also holds a pointer to its
 * reverse edge going from (v,u). Which is necessary when looking at the
 * residual network, G_f. This is also because the edges affect each others data
 * and thus u should have access to v and vice versa.
 * */
struct edge {
    int from;
    int to;
    int capacity;
    int flow;
    int original;
    struct edge* reverse_edge;
};

/* Initialize a forward and backward edge, holding information for G and G_f
 * Args:     - Integer symbolising the tail node, u in (u,v)
 *           - Integer symbolising the head node, v in (u,v)
 *           - The capacity of the edge
 * Return:   A pointer to the forward edge */
struct edge* edges_init(int from, int to, int c);

/* Frees edge and its reverse/backward edge
 * Args:     - Pointer to edge
 * Return:   Nothing */
void edge_free(struct edge*);

/* Prints the data from an edge
 * Args:     - A pointer to an edge but disguised as a void pointer
 * Return:   Nothing */
void edge_print(void*);

#endif

