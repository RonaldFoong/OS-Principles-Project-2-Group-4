#include "bestfit_lib.h"

#include "../utils/allocator_common.h"

#include <stdio.h>
#include <stdlib.h>

static alloc_list_t allocated_chunks = {NULL, NULL, 0U};
static alloc_list_t free_chunks = {NULL, NULL, 0U};

void *alloc(size_t chunk_size)
{
    alloc_node_t *node;
    alloc_node_t *best = NULL;
    allocation_t *allocation;
    size_t partition_size = partition_size_for(chunk_size);

    if (partition_size == 0U) {
        fprintf(stderr, "Error: allocation size must be between 1 and 512 bytes.\n");
        exit(EXIT_FAILURE);
    }

    // The smallest suitable free partition is the closest match
    for (node = free_chunks.head; node != NULL; node = node->next) {
        if (node->allocation->size >= chunk_size &&
            (best == NULL ||
             node->allocation->size < best->allocation->size)) {
            best = node;
        }
    }

    if (best != NULL) {
        allocation = list_remove(&free_chunks, best);
        allocation->used_size = chunk_size;
    } else {
        allocation = request_partition(partition_size, chunk_size);
    }

    append_or_exit(&allocated_chunks, allocation);
    return allocation->space;
}

void dealloc(void *chunk)
{
    alloc_node_t *node;

    for (node = allocated_chunks.head; node != NULL; node = node->next) {
        if (node->allocation->space == chunk) {
            allocation_t *allocation = list_remove(&allocated_chunks, node);

            allocation->used_size = 0U;
            append_or_exit(&free_chunks, allocation);
            return;
        }
    }

    fprintf(stderr, "Fatal error: pointer %p was not allocated by alloc().\n", chunk);
    exit(EXIT_FAILURE);
}

void allocator_print_state(void)
{
    print_allocated_list(&allocated_chunks);
    printf("Free list (%zu chunk%s):\n", free_chunks.length,
           free_chunks.length == 1U ? "" : "s");
    print_free_list(&free_chunks);
}

void allocator_destroy(void)
{
    list_destroy(&allocated_chunks, true);
    list_destroy(&free_chunks, true);
}
