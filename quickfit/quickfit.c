#include <stdio.h>
#include "quickfit_lib.h"
#include "../utils/allocator.h"
#include "../utils/print_list.h"

int main(int argc, char **argv) {
    // Parse and allocate data from datafile
    int result = allocator(argc, argv, "quickfit");
    if (result != 0) {
        return result;
    }

    // Print lists
    // Allocated chunks list
    printf("Allocated Chunks List: ");
    extern alloc_list_t allocated_list;
    print_alloc_list_contents(&allocated_list);

    // Free chunks lists
    printf("Free Chunks Lists:\n");
    extern alloc_list_t free_list[PARTITIONS];
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
                fprintf(stderr, "Error: Too many partition sizes");
                return 1;
        }
        print_free_list_contents(&free_list[i]);
    }

    // Free memory
    alloc_list_destroy(&allocated_list);
    for (int i = 0; i < PARTITIONS; i++) {
        alloc_list_destroy(&free_list[i]);
    }

    return 1;
}