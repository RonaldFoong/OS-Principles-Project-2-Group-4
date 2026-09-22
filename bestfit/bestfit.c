#include <stdio.h>
#include "bestfit_lib.h"
#include "../utils/allocator.h"
#include "../utils/print_list.h"

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
    print_alloc_list_contents(&allocated_chunks);

    // Free chunks list
    printf("Free Chunks List: ");
    extern alloc_list_t free_chunks;
    print_free_list_contents(&free_chunks);

    // Free memory
    alloc_list_destroy(&allocated_chunks);
    alloc_list_destroy(&free_chunks);

    return 1;
}