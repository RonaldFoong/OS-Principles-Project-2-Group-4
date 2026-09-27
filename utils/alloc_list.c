#include <stdio.h>
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
    return alloc_list_remove(self, self->tail);
}

alloc_node_t *alloc_list_find(alloc_list_t *self, void *space) {
    for (alloc_node_t *node = self->head; node != NULL; node = node->next) {
        if (node->alloc->space == space) {
            return node;
        }
    }
    return NULL;
}

allocation_t *alloc_list_remove(alloc_list_t *self, alloc_node_t *node) {
    /* Connect the neighbours to each other, skipping `node` */
    if (node->prev != NULL) {
        node->prev->next = node->next;
    } else {
        self->head = node->next;
    }
    if (node->next != NULL) {
        node->next->prev = node->prev;
    } else {
        self->tail = node->prev;
    }
    allocation_t *alloc = node->alloc;
    free(node);  /* free the node only - the allocation is kept */
    return alloc;
}

void alloc_list_print(alloc_list_t *self, bool show_used) {
    if (self->head == NULL) {
        printf("  (empty)\n");
        return;
    }
    for (alloc_node_t *node = self->head; node != NULL; node = node->next) {
        allocation_t *a = node->alloc;
        if (show_used) {
            printf("  address: %p  total size: %zu  used size: %zu\n",
                   a->space, a->total_size, a->used_size);
        } else {
            printf("  address: %p  total size: %zu\n", a->space, a->total_size);
        }
    }
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
    self->head = NULL;
    self->tail = NULL;
}
