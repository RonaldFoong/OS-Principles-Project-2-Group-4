#include <stdlib.h>
#include "alloc_list.h"

void push(alloc_list_t *self, allocation_t *alloc) {
    alloc_node_t *node = malloc(sizeof(alloc_node_t));
    node->next = NULL;
    node->alloc = alloc;
    if (self->tail == NULL) {
        self->head = node;
        self->tail = node;
        return;
    }
    self->tail->next = node;
    self->tail = node;
}

allocation_t *pop(alloc_list_t *self) {
    if (self->tail == NULL) {
        return NULL;
    }
    alloc_node_t *node = self->tail;
    allocation_t *popped = node->alloc;
    if (self->head == self->tail) {
        self->head = NULL;
        self->tail = NULL;
    } else {
        self->tail = self->tail->prev;
        self->tail->next = NULL;
    }
    free(node);
    return popped;
}