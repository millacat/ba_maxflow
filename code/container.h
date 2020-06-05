#ifndef CONTAINER_H
#define CONTAINER_H

/* A container element contains a pointer to 'this' element, which can be of
 * any type and a pointer to the next element in the container */
struct element {
    void* this;
    struct element* next;
};

/* Prints all the elements in the container from front to back
 * Args:    - A pointer to the first element of the container
 *          - A function pointer to use for printing the elements
 * Return:  Nothing */
void container_print(struct element*, void(void*));

/* Frees the elements of the container and the container itself
 * Args:    - A pointer to the container
 *          - A pointer to the head element of the container
 *          - Not 0: Free element->this. 0: Do not free element->this
 *          - Not 0: Free ((struct edge*)element)->this->reverse_edge. 0: Do not.
 * Return:  Nothing */
void container_free(void* container, struct element* head, int free_this,
                    int free_reverse);

#endif

