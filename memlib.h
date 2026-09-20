#ifndef MEMLIB_H
#define MEMLIB_H

#include <stddef.h>
#include "alloc_list.h"

void *alloc(size_t chunk_size);
void *dealloc(void *chunk);

#endif