# include "graph.h"
#include "maxflow.h"
#include "pushrelabel.h"
#include "loaddimacs.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <proc/readproc.h>

char* concat_strings(char* s1, char* s2) {
    char* res = malloc(strlen(s1) + strlen(s2) + 1);
    assert(res != NULL);
    strcpy(res, s1);
    strcat(res, s2);

    return res;
}

FILE* open_resfile(char* read_from, int maxflow) {
    char mf_str[120];
    // Build path to the result file
    sprintf(mf_str, "_mf%d_res", maxflow);
    char* file = concat_strings(read_from, mf_str);

    FILE* fp;
    fp = fopen(file, "w"); // Create the file
    assert(fp != NULL);
    free(file);

    return fp;
}

void write_results(char* alg, clock_t results[], FILE* fp, int len) {
    // Write name of algorithm to file using fp
    fprintf(fp, "%s\n", alg);

    // Write results for algorithm to file using fp
    for (int i = 0; i < len; i++) {
        fprintf(fp, "%ld\n", results[i]);
    }
}


int main(int argc, char* argv[]) {
    // Do time measuring for all three maximum flow algorithms
    // 3 times per algorithm, because it runs with different speed for each
    // iteration. Then the median can be found later.
    if (argc == 2) {
        int mf1, mf2, mf3, len = 3;
        // Must be sorted afterwards to find median. Done in plot.py.
        static clock_t time_measures[3];
        time_t start, end;
        struct graph* g;

        // Ford-Fulkerson
        for (int i = 0; i < len; i++) {
            g = read_dimacs_file(argv[1]);
            start = clock();
            mf1 = get_maximum_flow(g, &dfs);
            end = clock();
            time_measures[i] = end - start;
            graph_free(g);
        }

        FILE* fp = open_resfile(argv[1], mf1);
        write_results("dfs", time_measures, fp, len);

        // Edmonds-Karp
        for (int i = 0; i < len; i++) {
            g = read_dimacs_file(argv[1]);
            start = clock();
            mf2 = get_maximum_flow(g, &bfs);
            end = clock();
            time_measures[i] = end - start;
            graph_free(g);
        }
        write_results("bfs", time_measures, fp, len);

        // Relabel-to-Front
        for (int i = 0; i < len; i++) {
            g = read_dimacs_file(argv[1]);
            start = clock();
            mf3 = relabel_to_front(g);
            end = clock();
            time_measures[i] = end - start;
            graph_free(g);
        }

        write_results("rtf", time_measures, fp, len);

       // fprintf(stdout, "dfs %d, bfs %d, rtf %d\n", mf1, mf2, mf3);
        assert(mf1 == mf2);
        assert(mf2 == mf3);

        fclose(fp);

        return 0;
    }
    // Run one maximum flow algorithm once on graph
    // from the path in argv[1]
    else if (argc == 3) {
        struct graph* g;
        g = read_dimacs_file(argv[1]);
        int mf;

        if (strcmp(argv[2], "dfs") == 0) {
            mf = get_maximum_flow(g, &dfs);
        }
        else if (strcmp(argv[2],  "bfs") == 0) {
            mf = get_maximum_flow(g, &bfs);
        }
        else if (strcmp(argv[2], "rtf") == 0) {
            mf = relabel_to_front(g);
        }
        else {
            fprintf(stderr, "argv[2] should be bfs, dfs or rtf.\n"
                    "argv[2] was: %s\n", argv[2]);
        }
        graph_free(g);

        return 0;
    }
    else {
        fprintf(stderr, "Usage: ./runall [PATH]\n"
                "[PATH] is the DIMACS file to be run\n");
        return 1;
    }
}


/* This is four functions to return graphs manually build */
struct graph* build_g0() {
    struct graph* g = graph_init(6);
    //         graph, from, to, c
    graph_add_edges(g, 0, 1, 6);
    graph_add_edges(g, 0, 2, 7);
    graph_add_edges(g, 1, 2, 1);
    graph_add_edges(g, 1, 3,10);
    graph_add_edges(g, 2, 4, 8);
    graph_add_edges(g, 3, 2, 2);
    graph_add_edges(g, 3, 5,11);
    graph_add_edges(g, 4, 3, 6);
    graph_add_edges(g, 4, 5, 7);

    return g;
}

// Graph from p. 726 of CLRS
struct graph* build_g1() {
    struct graph* g = graph_init(6);
    graph_add_edges(g, 0, 1, 16);
    graph_add_edges(g, 0, 2, 13);
    graph_add_edges(g, 1, 3, 12);
    graph_add_edges(g, 2, 1,  4);
    graph_add_edges(g, 2, 4, 14);
    graph_add_edges(g, 3, 2,  9);
    graph_add_edges(g, 3, 5, 20);
    graph_add_edges(g, 4, 3,  7);
    graph_add_edges(g, 4, 5,  4);

    return g;
}

struct graph* build_g2() {
    struct graph* g = graph_init(4);
    graph_add_edges(g, 0, 1, 100);
    graph_add_edges(g, 0, 2, 100);
    graph_add_edges(g, 1, 2, 1);
    graph_add_edges(g, 1, 3, 100);
    graph_add_edges(g, 2, 3, 100);

    return g;
}

// Graph from p. 756 of CLRS
struct graph* build_g3() {
    struct graph* g = graph_init(5);
    graph_add_edges(g, 0, 1, 12);
    graph_add_edges(g, 0, 2, 14);
    graph_add_edges(g, 1, 2, 5);
    graph_add_edges(g, 1, 4, 16);
    graph_add_edges(g, 2, 3, 8);
    graph_add_edges(g, 3, 1, 7);
    graph_add_edges(g, 3, 4, 10);

    return g;
}
