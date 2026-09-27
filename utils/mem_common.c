#define _DEFAULT_SOURCE

#include "mem_common.h"
#include <stdio.h>
#include<stdlib.h>
#include <stdint.h>
#include <unistd.h>

const size_t PARTITION_SIZES[NUM_PARTITIONS] = {32, 64, 128, 256, 512};

int partition_index(size_t size)
{
    if(size == 0)
    {
        return -1;
    }

    for(int i = 0; i < NUM_PARTITIONS; ++i)
    {
        if(size <= PARTITION_SIZES[i])
        {
            return i;
        }
    }
    return -1;
}

allocation_t* new_chunk(size_t total_size)
{
    void* space = sbrk((intptr_t) total_size);

    if(space == (void *) -1)
    {
        fprintf(stderr, "Error: sbrk could not grow");
        return NULL;
    }

    allocation_t* record = malloc(sizeof(allocation_t));

    if(record == NULL)
    {
        fprintf(stderr, "Error:out of memory\n");
        return NULL;
    }

    record->total_size = total_size;
    record->used_size = 0;
    record->space = space;

    return record;
}