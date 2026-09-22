#ifndef ALLOC_LIST_H
#define ALLOC_LIST_H

#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    size_t total_size;
    size_t used_size;
    void *space;
} allocation_t;

typedef struct alloc_node_t {
    struct alloc_node_t *next;
    struct alloc_node_t *prev;
    allocation_t *alloc;
} alloc_node_t;

typedef struct {
    alloc_node_t *head;
    alloc_node_t *tail;
} alloc_list_t;

bool push(alloc_list_t *self, allocation_t *alloc);
allocation_t *pop(alloc_list_t *self);
void alloc_list_destroy(alloc_list_t *self);

#endif