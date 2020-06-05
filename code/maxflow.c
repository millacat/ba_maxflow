#include "maxflow.h"
#include "container.h"
#include "queue.h"
#include "list.h"
#include "edge.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Augment path with residual path capacity, cfp
void augment(struct list* path, int cfp) {
    struct element* cur = path->head;
    struct edge* cur_edge;

    while (cur) {
        cur_edge = (struct edge*)cur->this;
        if (cur_edge->original) {
            cur_edge->flow += cfp;
        }
        else {
            cur_edge->reverse_edge->flow -= cfp;
        }

        cur = cur->next;
    }
}

// Get the residual capacity of edge
int get_cfe(struct edge* e) {
    int cfe;

    /* (u,v) in E. */
    if (e->original) {
        cfe = e->capacity - e->flow;
    }
    /* (u,v) not in E. */
    else {
        cfe = e->reverse_edge->flow;
    }
    return cfe;
}

// Get the residual capacity of path:
// cfp = min{c_f(u,v) : (u,v) is on path)
int get_cfp(struct list* augpath) {
    struct element* cur = augpath->head;
    struct edge* cur_edge;
    int cfe;
    int min = INT_MAX;

    while (cur) {
        cur_edge = (struct edge*) cur->this;
        cfe = get_cfe(cur_edge);

        if (min > cfe) {
            min = cfe;
        }
        cur = cur->next;
    }
    return min;
}

// Build augmenting path using list.
// Uses the 'parents' array to find path.
struct list* build_augmenting_path(struct edge* parent[], int index) {
    struct list* augpath = list_init();

    while (index > 0) {

        struct edge* e = parent[index - 1];

        if (e) {
            list_insert_first(augpath, e);
        } else {
            // if edge on augmenting path is NULL there is no augmenting path
            container_free(augpath, augpath->head, 0, 0);
            return NULL;
        }
        index = e->from;
    }
    return augpath;
}

// Used for printing elements on the queue during bfs
void print_int_elm(void* x) {
    printf("| %d |\n", *(int*)x);
}


// Depth first search
struct list* dfs(struct graph* g) {
    int* visited = malloc(g->size * sizeof(int));

    struct edge* parent[g->size - 1];

    for (int i = 0; i < g->size; ++i) {
        visited[i] = -1;
        parent[i] = NULL;
    }

    for (int u = 0; u < g->size; u++) {
        if (visited[u] == -1) {
            dfs_visit(g, u, visited, parent);
        }
    }
    free(visited);

    // All vertices are visited and an augmenting path can be build
    // travelling  back from the sink to the source
    struct list* augmenting_path = build_augmenting_path(parent, g->size - 1);
    return augmenting_path;
}

void dfs_visit(struct graph* g, int u, int visited[], struct edge* parent[]) {
    visited[u] = 1;

    struct element* adj = g->adj_array[u]->head;
    struct edge* adj_e;
    int v, cfe;

    // Iterate u's adjacency list
    for (int i = 0; i < g->adj_array[u]->size; i++) {

        adj_e = (struct edge *)adj->this;
        cfe = get_cfe(adj_e);
        v = adj_e->to;

        // If v is not visited and there is residual capacity
        // on the edge (u,v) then set v's parent to u and visit v
        if (visited[v] == -1 && cfe) {
            parent[v - 1] = adj_e;
            dfs_visit(g, v, visited, parent);
        }
        adj = adj->next;
    }
}

struct list* bfs(struct graph* g) {

    int visited[g->size];
    struct edge* parents[g->size - 1];

    for (int i = 1; i < g->size; ++i) {
        //g->parents[i] = NULL;
        parents[i] = NULL;
        visited[i] = -1;
    }
    //g->parents[0] = NULL;
    parents[0] = NULL;
    visited[0] = 0;

    struct queue* q = queue_init();
    enqueue(q, &g->vertices[0]); // enqueue the source s

    while (!queue_is_empty(q)) {
        int u = *(int *)(dequeue(q));

        struct element* adj = g->adj_array[u]->head;
        int cfe;

        // Iterate dequeued element's, u's, adjacency list
        for (int i = 0; i < g->adj_array[u]->size; i++) {
            struct edge* v_edg = (struct edge*)adj->this;
            int v = v_edg->to;
            cfe = get_cfe(v_edg); // edge's residual capacity

            // If distances[v] is -1 v has not been visited before
            // If not visited before and if there is residual capacity on the
            // edge (u,v) then visit v, enqueue v and set its parent to u
            if ((visited[v] == -1) && (cfe > 0)) {
                visited[v] = 1;
                enqueue(q, &g->vertices[v]);
                parents[v - 1] = v_edg;
                //g->parents[v - 1] = v_edg;
            }
            adj = adj->next;
        }
    }
    container_free(q, q->head, 1, 0);

    struct list* augmenting_path = build_augmenting_path(parents, g->size - 1);
    //struct list* augmenting_path = build_augmenting_path(g->parents, g->size - 1);
    return augmenting_path;
}

int get_maximum_flow(struct graph* g,
                     struct list* search_algorithm(struct graph*)) {
    int cfp, maxflow;
    struct list* augpath;
    //int cnt;
    //cnt = 0;
    maxflow = 0;

    // Get an augmenting path using the file pointer
    // to a search algorithm (bfs or dfs)
    while ((augpath = (*search_algorithm)(g))) {
        cfp = get_cfp(augpath); // get residual path capacity
        augment(augpath, cfp);
        maxflow += cfp;
        container_free(augpath, augpath->head, 0, 0);
        //++cnt;
    }
//    printf("\nThe number of iterations to find maxflow is: %d\n\n", cnt);

    return maxflow;
}

