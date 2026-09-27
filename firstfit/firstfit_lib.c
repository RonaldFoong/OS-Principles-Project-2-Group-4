#include <stdio.h>
#include <stdlib.h>
#include "firstfit_lib.h"

static alloc_list_t allocated_list;
static alloc_list_t free_list;

void* alloc(size_t chunk_size)
{
    int index = partition_index(chunk_size);

    if(index == -1)
    {
        fprintf(stderr, "Error: cannot allocate %zu bytes (must be 1 - 512)\n", chunk_size);
        return NULL;
    }

    size_t needed = PARTITION_SIZES[index];

    allocation_t* chunk = NULL;

    for (alloc_node_t* node = free_list.head; 
        node != NULL; 
        node = node->next)
    {
        if(node->alloc->total_size >= needed)
        {
            chunk = alloc_list_remove(&free_list, node);
            break;
        }
    }

    if(chunk == NULL)
    {
        chunk = new_chunk(needed);

        if(chunk == NULL)
        {
            return NULL;
        }
    }

    chunk->used_size = chunk_size;

    if(!push(&allocated_list, chunk))
    {
        fprintf(stderr, "Error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    return chunk->space;
}