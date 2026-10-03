#ifndef ALLOC_LIST_H
#define ALLOC_LIST_H

#include <stdbool.h>
#include <stddef.h>

// Metadata is kept outside the simulated heap returned by sbrk()
typedef struct allocation {
    size_t size;       // Fixed partition size
    size_t used_size;  // Number of bytes requested by the caller
    void *space;       // Beginning of the partition
} allocation_t;

typedef struct alloc_node {
    struct alloc_node *next;
    struct alloc_node *prev;
    allocation_t *allocation;
} alloc_node_t;

typedef struct alloc_list {
    alloc_node_t *head;
    alloc_node_t *tail;
    size_t length;
} alloc_list_t;

bool list_append(alloc_list_t *list, allocation_t *allocation);
allocation_t *list_remove(alloc_list_t *list, alloc_node_t *node);
allocation_t *list_pop_back(alloc_list_t *list);
void list_destroy(alloc_list_t *list, bool free_allocations);

#endif