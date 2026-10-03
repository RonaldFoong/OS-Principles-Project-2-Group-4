#include <stdio.h>
#include <stdlib.h>
#include "quickfit_lib.h"


// Global Variables
static alloc_list_t allocated_list;
static alloc_list_t free_list[NUM_PARTITIONS];

void* alloc(size_t chunk_size)
{
    int index = partition_index(chunk_size);

    if(index == -1)
    {
        fprintf(stderr, "Error: cannot allocate %zu bytes (must be 1-512)\n", chunk_size);
        return NULL;
    }

    size_t needed = PARTITION_SIZES[index];

    allocation_t* chunk = NULL;

    if(free_list[index].head != NULL)
    {
        chunk = alloc_list_remove(&free_list[index], free_list[index].head);
    }
    else
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

void dealloc(void* chunk)
{
    alloc_node_t* node = alloc_list_find(&allocated_list, chunk);

    if(node ==NULL)
    {
        fprintf(stderr, "Fatal error: %p was never alocated\n", chunk);
        free_records();
        exit(EXIT_FAILURE);
    }

    allocation_t* record = alloc_list_remove(&allocated_list, node);

    record->used_size=0;

    int index = partition_index(record->total_size);

    if(!push(&free_list[index], record))
    {
        fprintf(stderr, "Error: out of memory\n");
        exit(EXIT_FAILURE);
    }
}

void print_memory(void)
{
    printf("Allocated list:\n");
    alloc_list_print(&allocated_list, true);

    for(int i  = 0; i < NUM_PARTITIONS; i++)
    {
        printf("Free list for %zu bytes chunks:\n", PARTITION_SIZES[i]);

        alloc_list_print(&free_list[i], false);
    }
}

void free_records(void)
{
    alloc_list_destroy(&allocated_list);

    for(int i = 0; i < NUM_PARTITIONS; ++i)
    {
        alloc_list_destroy(&free_list[i]);
    }
}