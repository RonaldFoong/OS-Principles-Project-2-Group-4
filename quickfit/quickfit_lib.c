#include <stddef.h>
#include "../utils/alloc_list.h"

alloc_list_t allocated_chunks;
alloc_list_t free_chunks_32;
alloc_list_t free_chunks_64;
alloc_list_t free_chunks_128;
alloc_list_t free_chunks_256;
alloc_list_t free_chunks_512;

void *alloc(size_t chunk_size) {

}

void *dealloc(void *chunk) {

}