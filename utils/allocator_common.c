#define _DEFAULT_SOURCE

#include "allocator_common.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static const size_t partition_sizes[] = {32U, 64U, 128U, 256U, 512U};
static const size_t partition_count =
    sizeof(partition_sizes) / sizeof(partition_sizes[0]);

size_t partition_size_for(size_t requested_size)
{
    size_t index;

    if (requested_size == 0U) {
        return 0U;
    }
    for (index = 0U; index < partition_count; index++) {
        if (requested_size <= partition_sizes[index]) {
            return partition_sizes[index];
        }
    }
    return 0U;
}

allocation_t *request_partition(size_t partition_size, size_t used_size)
{
    allocation_t *allocation = malloc(sizeof(*allocation));
    void *space;

    if (allocation == NULL) {
        fprintf(stderr, "Error: unable to allocate partition metadata.\n");
        exit(EXIT_FAILURE);
    }
    space = sbrk((intptr_t)partition_size);
    if (space == (void *)-1) {
        free(allocation);
        fprintf(stderr, "Error: sbrk could not grow the heap.\n");
        exit(EXIT_FAILURE);
    }
    allocation->size = partition_size;
    allocation->used_size = used_size;
    allocation->space = space;
    return allocation;
}

void append_or_exit(alloc_list_t *list, allocation_t *allocation)
{
    if (!list_append(list, allocation)) {
        fprintf(stderr, "Error: unable to allocate a linked-list node.\n");
        exit(EXIT_FAILURE);
    }
}

void print_allocated_list(const alloc_list_t *list)
{
    const alloc_node_t *node;

    printf("Allocated list (%zu chunk%s):\n", list->length,
           list->length == 1U ? "" : "s");
    for (node = list->head; node != NULL; node = node->next) {
        printf("  address=%p total_size=%zu used_size=%zu\n",
               node->allocation->space, node->allocation->size,
               node->allocation->used_size);
    }
    if (list->head == NULL) {
        printf("  (empty)\n");
    }
}

void print_free_list(const alloc_list_t *list)
{
    const alloc_node_t *node;

    for (node = list->head; node != NULL; node = node->next) {
        printf("  address=%p total_size=%zu\n", node->allocation->space,
               node->allocation->size);
    }
    if (list->head == NULL) {
        printf("  (empty)\n");
    }
}
