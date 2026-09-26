#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include "../utils/alloc_list.h"
#include "firstfit_lib.h"

static alloc_list_t allocated_list;
static alloc_list_t free_list;

static size_t get_partition_size(size_t chunk_size) {
    if (chunk_size <= 32)
        return 32;
    else if (chunk_size <= 64)
        return 64;
    else if (chunk_size <= 128)
        return 128;
    else if (chunk_size <= 256)
        return 256;
    else if (chunk_size <= 512)
        return 512;
    else
        return 0;
}

void *alloc(size_t chunk_size) {
    size_t partition_size = get_partition_size(chunk_size);
    if (partition_size == 0)
        return NULL;

    // search free list
    alloc_node_t *node = free_list.head;
    while (node != NULL) {
        if (node->alloc->total_size == partition_size)
            break;
        node = node->next;
    }

    if (node != NULL) {
        // reuse from free list
        if (node->prev != NULL)
            node->prev->next = node->next;
        else
            free_list.head = node->next;

        if (node->next != NULL)
            node->next->prev = node->prev;
        else
            free_list.tail = node->prev;

        void *result = node->alloc->space;
        node->alloc->used_size = chunk_size;
        push(&allocated_list, node->alloc);
        free(node);
        return result;
    }

    // grow heap
    void *space = sbrk(partition_size);
    if (space == (void *)-1)
        return NULL;

    allocation_t *new_alloc = malloc(sizeof(allocation_t));
    new_alloc->space = space;
    new_alloc->total_size = partition_size;
    new_alloc->used_size = chunk_size;
    push(&allocated_list, new_alloc);
    return space;
}

void dealloc(void *chunk) {

}
