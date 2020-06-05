#include "container.h"
#include "edge.h"

#include <stdio.h>
#include <stdlib.h>

void container_print(struct element* head, void (*elm_func)(void*)) {
    struct element* cur = head;

    if (!head) { // head is NULL
        printf("The container is empty.\n");
    } else {
        while (cur) {
            // Use function pointer to call *elm_func with cur->this as argument
            (*elm_func)(cur->this);
            cur = cur->next;
        }
        printf("\n");
    }
}

void container_free(void* container, struct element* head, int free_this,
                                                           int free_reverse) {
    struct element* cur = head;

    while (cur) {
        struct element* next = cur->next;
        // If free_reverse is flagged then element->this is an edge struct and
        // the reverse edge is freed
        if (free_reverse) {
            free(((struct edge*)cur->this)->reverse_edge);
        }
        if (free_this) {
            free(cur->this);
        }
        free(cur);
        cur = next;
    }
    free(container);
}

