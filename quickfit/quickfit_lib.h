#ifndef QUICKFIT_LIB_H
#define QUICKFIT_LIB_H

#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "../utils/alloc_list.h"
#include "../utils/chunk_size_of.h"

#define PARTITIONS 5

alloc_list_t allocated_list;
alloc_list_t free_list[PARTITIONS];

typedef enum {
    BYTES_32,
    BYTES_64,
    BYTES_128,
    BYTES_256,
    BYTES_512,
    BYTES_LARGE
} list_type_t;

void *alloc(size_t chunk_size);
void dealloc(void *chunk);
list_type_t get_list_type(size_t bytes);

#endif