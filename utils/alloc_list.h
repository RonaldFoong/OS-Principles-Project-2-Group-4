#ifndef ALLOC_LIST_H
#define ALLOC_LIST_H

#include <stdlib.h>
#include <stdbool.h>

/* Information about one chunk of memory */
typedef struct {
    size_t total_size;  /* chunk size: 32, 64, 128, 256 or 512 */
    size_t used_size;   /* bytes the user asked for (0 when free) */
    void *space;        /* where the chunk starts */
} allocation_t;

/* One node in a doubly linked list */
typedef struct alloc_node_t {
    struct alloc_node_t *next;
    struct alloc_node_t *prev;
    allocation_t *alloc;
} alloc_node_t;

/* A list = first node + last node. Both NULL means empty. */
typedef struct {
    alloc_node_t *head;
    alloc_node_t *tail;
} alloc_list_t;

/* Add to the end of the list. Returns false if out of memory. */
bool push(alloc_list_t *self, allocation_t *alloc);

/* Remove and return the last item (NULL if empty). */
allocation_t *pop(alloc_list_t *self);

/* Find the node whose chunk starts at `space` (NULL if not found). */
alloc_node_t *alloc_list_find(alloc_list_t *self, void *space);

/* Take `node` out of the list and return its allocation. */
allocation_t *alloc_list_remove(alloc_list_t *self, alloc_node_t *node);

/* Print every chunk. show_used = also print the used size. */
void alloc_list_print(alloc_list_t *self, bool show_used);

/* Free every node and allocation, leaving an empty list. */
void alloc_list_destroy(alloc_list_t *self);

#endif
