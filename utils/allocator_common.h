#ifndef ALLOCATOR_COMMON_H
#define ALLOCATOR_COMMON_H

#include <stddef.h>

#include "alloc_list.h"

size_t partition_size_for(size_t requested_size);
allocation_t *request_partition(size_t partition_size, size_t used_size);
void append_or_exit(alloc_list_t *list, allocation_t *allocation);
void print_allocated_list(const alloc_list_t *list);
void print_free_list(const alloc_list_t *list);

#endif
