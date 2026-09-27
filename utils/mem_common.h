#ifndef MEM_COMMON_H
#define MEM_COMMON_H

#include <stddef.h>
#include "alloc_list.h"

#define NUM_PARTITIONS 5

extern const size_t PARTITION_SIZES[NUM_PARTITIONS];

int partition_index(size_t size);

allocation_t* new_chunk(size_t total_size);

void* alloc(size_t chunk_size);
#endif