#include "quickfit_lib.h"

#include "../utils/allocator_common.h"

#include <stdio.h>
#include <stdlib.h>

#define QUICK_LIST_COUNT 5U

static const size_t quick_sizes[QUICK_LIST_COUNT] =
    {32U, 64U, 128U, 256U, 512U};
static alloc_list_t allocated_chunks = {NULL, NULL, 0U};
static alloc_list_t free_chunks[QUICK_LIST_COUNT];

static size_t list_index_for(size_t partition_size)
{
    size_t index;

    for (index = 0U; index < QUICK_LIST_COUNT; index++) {
        if (quick_sizes[index] == partition_size) {
            return index;
        }
    }
    fprintf(stderr, "Fatal error: invalid partition size %zu.\n", partition_size);
    exit(EXIT_FAILURE);
}

void *alloc(size_t chunk_size)
{
    allocation_t *allocation;
    size_t partition_size = partition_size_for(chunk_size);
    size_t index;

    if (partition_size == 0U) {
        fprintf(stderr, "Error: allocation size must be between 1 and 512 bytes.\n");
        exit(EXIT_FAILURE);
    }
    index = list_index_for(partition_size);

    // Quick fit performs O(1) selection of the exact-size free list
    allocation = list_pop_back(&free_chunks[index]);
    if (allocation == NULL) {
        allocation = request_partition(partition_size, chunk_size);
    } else {
        allocation->used_size = chunk_size;
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
            size_t index = list_index_for(allocation->size);

            allocation->used_size = 0U;
            append_or_exit(&free_chunks[index], allocation);
            return;
        }
    }

    fprintf(stderr, "Fatal error: pointer %p was not allocated by alloc().\n", chunk);
    exit(EXIT_FAILURE);
}

void allocator_print_state(void)
{
    size_t index;

    print_allocated_list(&allocated_chunks);
    printf("Quick-fit free lists:\n");
    for (index = 0U; index < QUICK_LIST_COUNT; index++) {
        printf("Partition %zu bytes (%zu chunk%s):\n", quick_sizes[index],
               free_chunks[index].length,
               free_chunks[index].length == 1U ? "" : "s");
        print_free_list(&free_chunks[index]);
    }
}

void allocator_destroy(void)
{
    size_t index;

    list_destroy(&allocated_chunks, true);
    for (index = 0U; index < QUICK_LIST_COUNT; index++) {
        list_destroy(&free_chunks[index], true);
    }
}
