#include <stdio.h>
#include "bestfit_lib.h"
#include "../utils/allocator.h"

int main(int argc, char **argv) {
    // Parse and allocate data from datafile
    int result = allocator(argc, argv, "bestfit");
    if (result != 0) {
        return result;
    }

    // Print lists
    // Allocated chunks list
    printf("Allocated Chunks List: ");
    extern alloc_list_t allocated_chunks;
    alloc_node_t *node = allocated_chunks.head;
    if (node != NULL) {
        printf(
            "{addr: %p, total size: %zu, used size: %zu}", 
            node->alloc->space, 
            node->alloc->total_size, 
            node->alloc->used_size
        );
        node = node->next;
    } else {
        printf("{}");
    }
    for (; node != NULL; node = node->next) {
        printf(
            " -> {addr: %p, total size: %zu, used size: %zu}", 
            node->alloc->space, 
            node->alloc->total_size, 
            node->alloc->used_size
        );
    }
    printf("\n");

    // Free chunks list
    printf("Free Chunks List: ");
    extern alloc_list_t free_chunks;
    node = free_chunks.head;
    if (node != NULL) {
        printf(
            "{addr: %p, total size: %zu}", 
            node->alloc->space, 
            node->alloc->total_size
        );
        node = node->next;
    } else {
        printf("{}");
    }
    for (; node != NULL; node = node->next) {
        printf(
            " -> {addr: %p, total size: %zu}", 
            node->alloc->space, 
            node->alloc->total_size
        );
    }
    printf("\n");

    return 1;
}