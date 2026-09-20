#ifndef FIRSTFIT_LIB_H
#define FIRSTFIT_LIB_H

#include <stddef.h>
#include "../utils/alloc_list.h"

void *alloc(size_t chunk_size);
void dealloc(void *chunk);

#endif