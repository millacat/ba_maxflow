#define _GNU_SOURCE
#include "loaddimacs.h"
#include "graph.h"

#include "stdio.h"
#include "stdlib.h"
#include "assert.h"

struct graph* read_and_build_graph(char* line) {
    int V, E, offset = 5;
    sscanf(line + offset, "%d %d", &V, &E);

    struct graph* g = graph_init(V);
    return g;
}

void read_and_add_edge(struct graph* g, char* line) {
    int from, to, capacity, offset = 2;
    sscanf(line + offset, "%d %d %d", &from, &to, &capacity);
    graph_add_edges(g, from - 1, to - 1, capacity);
}

// Use this if running on other peoples DIMACS files
// The difference is that I always give the source number 0 and the sink the
// last number, |V| - 1.
void read_and_add_edge2(struct graph* g, char* line, int sink, int last_index) {
    int from, to, capacity, offset = 2;
    sscanf(line + offset, "%d %d %d", &from, &to, &capacity);

    if (capacity > 0) {
        if (from == sink) {
            from = last_index;
        }
        else if (from == last_index) {
            from = sink;
        }

        if (to == last_index) {
            to = sink;
        }
        else if (to == sink) {
            to = last_index;
        }

        graph_add_edges(g, from - 1, to - 1, capacity);
    }
}

struct graph* read_dimacs_file(char* file) {
    FILE* fp;
    fp = fopen(file, "r");
    assert(fp != NULL);

    struct graph* g;
    int source, sink, ans;
    char c[1];
    char* line = NULL;
    size_t chars_read, len = 0;

    // Read one line at a time
    while((chars_read = getline(&line, &len, fp) != EOF)) {

        switch (line[0]) {
            case 'a': // Edge
                read_and_add_edge(g, line);
                break;
            case 'c': // Comment
                break;
            case 'n': // Sorce or sink info
                sscanf(line + 2, "%d %c", &ans, c);
                if (*c == 's') {
                    source = ans;
                } else {
                    sink = ans;
                }
                break;
            case 'p': // Graph info: |V| and |E|
                g = read_and_build_graph(line);
                break;
        }
    }

    free(line);
    int closed = fclose(fp);
    assert(closed == 0);
    return g;
}

