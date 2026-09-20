#ifndef FIRSTFIT_LIB_H
#define FIRSTFIT_LIB_H

#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "../utils/alloc_list.h"
#include "../utils/chunk_size_of.h"

void *alloc(size_t chunk_size);
void dealloc(void *chunk);

#endif