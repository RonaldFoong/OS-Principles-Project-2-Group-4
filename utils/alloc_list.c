#include <stdlib.h>
#include <stdbool.h>
#include "alloc_list.h"

bool push(alloc_list_t *self, allocation_t *alloc) {
    alloc_node_t *node = malloc(sizeof(alloc_node_t));
    if (node == NULL) {
        return false;
    }
    node->next = NULL;
    node->prev = self->tail;
    node->alloc = alloc;
    if (self->tail == NULL) {
        self->head = node;
        self->tail = node;
        return true;
    }
    self->tail->next = node;
    self->tail = node;
    return true;
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

allocation_t *find_and_remove(alloc_list_t *self, void *space) {
    alloc_node_t *node = self->head;

    while (node != NULL) {
        if (node->alloc->space == space)
            break;
        node = node->next;
    }

    if (node == NULL)
        return NULL;

    if (node->prev != NULL)
        node->prev->next = node->next;
    else
        self->head = node->next;

    if (node->next != NULL)
        node->next->prev = node->prev;
    else
        self->tail = node->prev;

    allocation_t *alloc = node->alloc;
    free(node);
    return alloc;
}

void alloc_list_destroy(alloc_list_t *self) {
    alloc_node_t *node = self->head;
    alloc_node_t *next = NULL;
    while (node != NULL) {
        free(node->alloc);
        next = node->next;
        free(node);
        node = next;
    }
}