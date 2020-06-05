#ifndef LOADDIMACS_H
#define LOADDIMACS_H

/* Builds a graph based on a DIMACS file
 * Args:    - The path to the file to load
 * Return:  Pointer to graph */
struct graph* read_dimacs_file(char* file);

/* Initializes a graph based on the input line
 * Args:    - A line with info about |V| and |E|
 * Return:  Pointer to graph */
struct graph* read_and_build_graph(char* line);


/* Adds an edge to the graph based on the input line
 * Args:    - Pointer to graph
 *          - Line with info about vertices to add edge between and its capacity
 * Return:  Nothing */
void read_and_add_edge(struct graph* g, char* line);

/* Adds an edge to the graph based on the input line
 * If the sinks current number emerges it is changed to the last index in the
 * graph and vice versa.
 * Args:    - Pointer to graph
 *          - Line with info about vertices to add edge between and its capacity
 *          - The sinks current number
 *          - The last index in the graph, |V| - 1
 * Return:  Nothing */
void read_and_add_edge2(struct graph* g, char* line, int sink,
                        int last_index);
#endif

