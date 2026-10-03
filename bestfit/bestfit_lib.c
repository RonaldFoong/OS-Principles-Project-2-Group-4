#include <stdio.h>
#include <stdlib.h>
#include "bestfit_lib.h"

// Global Variables
static alloc_list_t allocated_list;
static alloc_list_t free_list;

void* alloc(size_t chunk_size)
{
    int index = partition_index(chunk_size);

    if(index == -1)
    {
        fprintf(stderr, "Error: cannot allocate %zu bytes (must be 1-512)\n",
                chunk_size);
        return NULL;
    }

    size_t needed = PARTITION_SIZES[index];

    alloc_node_t* best =NULL;
    for(alloc_node_t* node = free_list.head; node != NULL; node = node->next)
    {
        size_t size = node->alloc->total_size;

        if(size >= needed && (best ==NULL || size < best->alloc->total_size))
        {
            best = node;
        }
    }

    allocation_t* chunk = NULL;
    if(best != NULL)
    {
        chunk = alloc_list_remove(&free_list, best);
    }
    else
    {
        chunk = new_chunk(needed);
    }

    if(chunk == NULL)
    {
        return NULL;
    }

    chunk->used_size = chunk_size;

    if(!push(&allocated_list, chunk))
    {
        fprintf(stderr,"Error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    return chunk->space;
}

void dealloc(void* chunk)
{
    alloc_node_t* node = alloc_list_find(&allocated_list, chunk);

    if(node == NULL)
    {
        fprintf(stderr, "Fatal error: %p was never allocated\n", chunk);
        free_records();
        exit(EXIT_FAILURE);
    }
    allocation_t* record = alloc_list_remove(&allocated_list, node);

    record->used_size = 0;

    if(!push(&free_list, record))
    {
        fprintf(stderr, "Error: out of memory\n");
        exit(EXIT_FAILURE);
    }
}

void free_records(void)
{
    alloc_list_destroy(&allocated_list);
    alloc_list_destroy(&free_list);
}

void print_memory(void)
{
    printf("Allocated list:\n");
    alloc_list_print(&allocated_list, true);
    printf("Free list:\n");
    alloc_list_print(&free_list, false);
}