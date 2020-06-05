#include "list.h"
#include "container.h"
#include "edge.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

list* list_init() {
    struct list* l = malloc(sizeof(list));
    assert(l != NULL);

    if (l) {
        l->head = NULL;
    }
    l->size = 0;
    return l;
}

int list_insert_first(list* l, void* ins_elm) {
    struct element* new_elm = malloc(sizeof(struct element));
    assert(new_elm != NULL);

    if (new_elm) {
        new_elm->this = ins_elm;
        new_elm->next = l->head;
        l->head = new_elm;
        ++l->size;
        return 0;
    } else {
        return 1;
    }
}

int list_insert_last(struct list* l, void* elm) {
    // If head element is null the element is inserted at front of list
    if (!l->head) {
        return list_insert_first(l, elm);
    }

    struct element* prev = l->head;
    struct element* cur  = prev->next;
    // Iterate until cur == NULL and prev is the last element in l
    while (cur) {
       prev = cur;
       cur = cur->next;
    }

    struct element* new_elm = malloc(sizeof(struct element));
    assert(new_elm != NULL);

    if (new_elm) {
        new_elm->this = elm;
        new_elm->next = NULL;
        prev->next = new_elm;
        ++l->size;
        return 0; // success
    } else {
        return 1; // failure
    }
}

void* list_remove_element(list* l, int target_index) {
    assert(target_index >= 0);
    assert(l->head != NULL);

    if (target_index == 0) {
        struct element* former_head = l->head;
        struct element* ret_elm = former_head->this;

        // Set new head
        l->head = l->head->next;
        free(former_head);
        --l->size;
        return ret_elm;
    }

    assert(l->head->next != NULL);

    struct element* prev = l->head;
    struct element* cur  = prev->next;

    target_index--;

    // Iterate until index is reached
    while (target_index > 0 && cur) {
        prev = cur;
        cur  = cur->next;
        target_index--;
    }

    void* ret_elm = cur->this; // Unwrap element
    prev->next = cur->next;    // Cut 'cur' element out of list
    free(cur);

    --l->size;
    return ret_elm;
}

void list_move_to_front(struct list* l, int index) {
    assert(index >= 0);
    assert(l->head != NULL);

    if (0 < index) {
        struct element* prev = l->head;
        struct element* cur  = prev->next;

        --index;
        // Iterate until index is reached
        while (0 < index && cur) {
            prev = cur;
            cur = cur->next;
            --index;
        }
        prev->next = cur->next; // Cut 'cur' out of list
        cur->next = l->head;    // Insert it at front of list
        l->head = cur;
    }
}

