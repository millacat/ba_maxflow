#include "pushrelabel.h"
#include "maxflow.h"
#include "edge.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

// Only checks residual capacity of edge and the heights since excess is
// checked earlier in push relabel algorithm.
int push_applies(struct edge* uv, int* heights) {
    return get_cfe(uv) > 0 && heights[uv->from] == heights[uv->to] + 1;
}

void push(struct edge* uv, int* excess) {
    int ue = excess[uv->from];
    int cfe = get_cfe(uv);
    int min = ue < cfe ? ue : cfe;

    // Pushing flow
    uv->original ? (uv->flow += min) : (uv->reverse_edge->flow -= min);

    // Updating excess flow for u and v
    excess[uv->from] = ue - min;
    excess[uv->to]   = excess[uv->to] + min;
}

// Do actually not need this function, because if relabel is reached in
// relabel_to_front then relabel already applies, because L's end is reached
// and the excess of u is positive.
int relabel_applies(int u, int* excess, struct list* neighbors, int* heights) {
    int ans;
    ans = excess[u] > 0;
    if (!ans) {
        return ans;
    }

    struct element* cur = neighbors->head;
    struct edge* edge;
    int cnt = 0;

    while (ans && cnt < neighbors->size) {
        edge = ((struct edge*)cur->this);

        if (get_cfe(edge) > 0) {
            ans = ans && heights[u] <= heights[edge->to];
        }

        ++cnt;
        cur = cur->next;
    }

    return ans;
}

void relabel(int u, struct list* neighbors, int* heights) {
    int min = INT_MAX;
    int cfe, v_height;

    struct element* cur = neighbors->head;

    for (int i = 0; i < neighbors->size; i++) {
        cfe = get_cfe(cur->this);
        v_height = heights[((struct edge*)cur->this)->to];
        // The edge must be part of the residual graph, being 0 < cfe is true.
        if (0 < cfe && min > v_height) {
           min = v_height;
        }
        cur = cur->next;
    }
    // u.height = 1 + min{v.h : (u,v) in E_f}
    heights[u] = 1 + min;
}

void initialize_preflow(struct graph* g, int* excess, int* height) {
    int s = 0;

    for (int i = 0; i < g->size; i++) {
        excess[i] = 0;
        height[i] = 0;
    }
    height[s] = g->size;

    struct list* s_adj = g->adj_array[s];
    struct element* cur = s_adj->head;
    struct edge* cur_e;

    // Iterate the source's adjacency list and push flow to all its neighbors
    // so the edges get saturated
    for (int i = 0; i < s_adj->size; i++) {

        cur_e = ((struct edge*)cur->this);

        cur_e->flow = cur_e->capacity;
        excess[cur_e->to] = cur_e->capacity;
        excess[s] -= cur_e->capacity;

        cur = cur->next;
    }
}

// Implementaion of Relabel-to-front algorithm from p. 755 of CLRS
int relabel_to_front(struct graph* g) {
    int* height = malloc(g->size * sizeof(int));
    int* excess = malloc(g->size * sizeof(int));
    int v_except_s_t = g->size - 2;
    struct element** cur_pointers = malloc(v_except_s_t * sizeof(struct element));
    struct list* L = list_init();

    for (int i = 0; i < v_except_s_t; i++) {
        // L = G.V - {s,t}
        list_insert_last(L, &g->vertices[i+1]);
        // The neighbor list of an vertex, u.N, is the same as its adjacency
        // list in this implementation
        //for each vertex - s,t do u.current = u.N.head
        cur_pointers[i] = g->adj_array[i+1]->head;
    }

    initialize_preflow(g, excess, height);

    struct element* u = L->head;

    int old_height, u_data, index = 0;

    // Traverse L
    while (u) {
        // get the integer representing vertex u
        u_data = *((int*)u->this);
        old_height = height[u_data];

        // Discharge u
        while (excess[u_data] > 0) {
            // v = u.current
            struct element* v = cur_pointers[u_data - 1];

            // Relabel
            // u's neighbors list's end is reached and none of u's outgoing
            // edges are admissible
            if (v == NULL) {
                relabel(u_data, g->adj_array[u_data], height);
                cur_pointers[u_data - 1] = g->adj_array[u_data]->head;
            }
            // Push
            else if (push_applies(((struct edge*)v->this), height)) {
                push((struct edge*)v->this, excess);
            }
            // Neither push or relabel applies.
            // Move u.current to its next neighbor
            else {
                cur_pointers[u_data - 1] = v->next;
            }
        }

        if (height[u_data] > old_height) {
            // Move u to front of L and reset index
            list_move_to_front(L, index);
            index = 0;
        }

        ++index;
        u = u->next;
    }

    int maximum_flow = excess[g->size-1];

    container_free(L, L->head, 0, 0);
    free(cur_pointers);
    free(height);
    free(excess);

    return maximum_flow;
}

