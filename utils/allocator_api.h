#ifndef ALLOCATOR_API_H
#define ALLOCATOR_API_H

#include <stddef.h>

void *alloc(size_t chunk_size);
void dealloc(void *chunk);
void allocator_print_state(void);
void allocator_destroy(void);

#endif
