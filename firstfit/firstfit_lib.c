#include <stddef.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include "../utils/alloc_list.h"
#include "../utils/chunk_size_of.h"

alloc_list_t allocated_chunks;
alloc_list_t free_chunks;

void *alloc(size_t chunk_size) {
    for (alloc_node_t *node = free_chunks.head; node != NULL; node = node->next) {
        if (node->alloc->size >= chunk_size) {
            if (node->prev != NULL && node->next != NULL) {
                node->prev->next = node->next;
                node->next->prev = node->prev;
            } else if (node->prev != NULL) {
                node->prev->next = NULL;
            } else if (node->next != NULL) {
                node->next->prev = NULL;
            }

            allocation_t *alloc = node->alloc;
            free(node);
            if (!push(&allocated_chunks, alloc)) {
                return NULL;
            }
            return alloc->space;
        }
    }

    size_t size = chunk_size_of(chunk_size);
    if (size == 0) {
        return NULL;
    }

    void *space = sbrk(size);
    if (space == (void *)-1) {
        return NULL;
    }

    allocation_t *alloc = malloc(sizeof(allocation_t));
    if (alloc == NULL) {
        return NULL;
    }

    alloc->size = size;
    alloc->space = space;
    if (!push(&allocated_chunks, alloc)) {
        return NULL;
    }

    return space;
}

void *dealloc(void *chunk) {

}