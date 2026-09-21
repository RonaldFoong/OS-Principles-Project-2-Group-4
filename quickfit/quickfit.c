#include <stdio.h>
#include "quickfit_lib.h"
#include "../utils/allocator.h"

int main(int argc, char **argv) {
    // Parse and allocate data from datafile
    int result = allocator(argc, argv, "quickfit");
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

    // Free chunks lists
    printf("Free Chunks Lists:\n");
    extern alloc_list_t free_chunks[PARTITIONS];
    for (list_type_t i = 0; i < PARTITIONS; i++) {
        switch (i) {
            case BYTES_32:
                printf("\tBYTES_32: ");
                break;
            case BYTES_64:
                printf("\tBYTES_64: ");
                break;
            case BYTES_128:
                printf("\tBYTES_128: ");
                break;
            case BYTES_256:
                printf("\tBYTES_256: ");
                break;
            case BYTES_512:
                printf("\tBYTES_512: ");
                break;
            case BYTES_LARGE:
                break;
        }

        node = free_chunks[i].head;
        if (node != NULL) {
            printf(
                "{addr: %p, total size: %zu}", 
                node->alloc->space, 
                node->alloc->total_size
            );
            node = node->next;
        }
        for (; node != NULL; node = node->next) {
            printf(
                " -> {addr: %p, total size: %zu}", 
                node->alloc->space, 
                node->alloc->total_size
            );
        }
        printf("\n");
    }

    return 1;
}