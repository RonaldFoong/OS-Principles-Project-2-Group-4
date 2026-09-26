#define _GNU_SOURCE
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../utils/alloc_list.h"
#include "quickfit_lib.h"

static alloc_list_t allocated_list;
static alloc_list_t free_list[5];

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

static int get_free_list_index(size_t partition_size) {
    if (partition_size == 32)
        return 0;
    else if (partition_size == 64)
        return 1;
    else if (partition_size == 128)
        return 2;
    else if (partition_size == 256)
        return 3;
    else if (partition_size == 512)
        return 4;
    else
        return -1;
}

void *alloc(size_t chunk_size) {
    size_t partition_size = get_partition_size(chunk_size);
    if (partition_size == 0)
        return NULL;

    int idx = get_free_list_index(partition_size);

    allocation_t *reused = pop(&free_list[idx]);
    if (reused != NULL) {
        reused->used_size = chunk_size;
        push(&allocated_list, reused);
        return reused->space;
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
    allocation_t *alloc = find_and_remove(&allocated_list, chunk);

    if (alloc == NULL) {
        fprintf(stderr, "Invalid chunk\n");
        exit(EXIT_FAILURE);
    }

    int idx = get_free_list_index(alloc->total_size);
    push(&free_list[idx], alloc);
}

void print_lists(void) {
    alloc_node_t *node;

    printf("Allocated:\n");
    node = allocated_list.head;
    while (node != NULL) {
        printf("address: %p, used: %zu, total: %zu\n",
               node->alloc->space,
               node->alloc->used_size,
               node->alloc->total_size);
        node = node->next;
    }

    size_t sizes[5] = {32, 64, 128, 256, 512};
    for (int i = 0; i < 5; i++) {
        printf("Free (%zu bytes):\n", sizes[i]);
        node = free_list[i].head;
        while (node != NULL) {
            printf("address: %p, total: %zu\n",
                   node->alloc->space,
                   node->alloc->total_size);
            node = node->next;
        }
    }
}
