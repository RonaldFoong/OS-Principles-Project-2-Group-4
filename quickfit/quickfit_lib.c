#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "../utils/alloc_list.h"
#include "../utils/chunk_size_of.h"
#include "quickfit_lib.h"

alloc_list_t allocated_chunks;
alloc_list_t free_chunks[5];

void *alloc(size_t chunk_size) {
    list_type_t list_type = get_list_type(chunk_size);
    if (list_type == BYTES_LARGE) {
        fprintf(stderr, "Error: Chunk size is too large");
        exit(EXIT_FAILURE);
    }
    
    if (free_chunks[list_type].head != NULL) {
        alloc_node_t *node = free_chunks[list_type].head;
        if (node->prev != NULL && node->next != NULL) {
            node->prev->next = node->next;
            node->next->prev = node->prev;
        } else if (node->prev != NULL) {
            node->prev->next = NULL;
        } else if (node->next != NULL) {
            node->next->prev = NULL;
        }

        allocation_t *alloc = node->alloc;
        alloc->used_size = chunk_size;
        free(node);
        if (!push(&allocated_chunks, alloc)) {
            fprintf(stderr, "Error: Could not allocate chunk");
            exit(EXIT_FAILURE);
        }

        return alloc->space;
    }

    size_t total_size = chunk_size_of(chunk_size);
    if (total_size == 0) {
        fprintf(stderr, "Error: Chunk size is too large");
        exit(EXIT_FAILURE);
    }

    void *space = sbrk(total_size);
    if (space == (void *)-1) {
        fprintf(stderr, "Error: Could not allocate chunk");
        exit(EXIT_FAILURE);
    }

    allocation_t *alloc = malloc(sizeof(allocation_t));
    if (alloc == NULL) {
        fprintf(stderr, "Error: Could not allocate memory");
        exit(EXIT_FAILURE);
    }

    alloc->total_size = total_size;
    alloc->used_size = chunk_size;
    alloc->space = space;
    if (!push(&allocated_chunks, alloc)) {
        fprintf(stderr, "Error: Could not allocate chunk");
        exit(EXIT_FAILURE);
    }

    return space;
}

void dealloc(void *chunk) {
    for (alloc_node_t *node = allocated_chunks.head; node != NULL; node = node->next) {
        if (node->alloc->space == chunk) {
            if (node->prev != NULL && node->next != NULL) {
                node->prev->next = node->next;
                node->next->prev = node->prev;
            } else if (node->prev != NULL) {
                node->prev->next = NULL;
            } else if (node->next != NULL) {
                node->next->prev = NULL;
            }

            allocation_t *alloc = node->alloc;
            alloc->used_size = 0;
            free(node);
            list_type_t list_type = get_list_type(alloc->total_size);
            if (!push(&free_chunks[list_type], alloc)) {
                fprintf(stderr, "Error: Could not deallocate chunk");
                exit(EXIT_FAILURE);
            }
            return;
        }
    }
    fprintf(stderr, "Error: Chunk does not exist");
    exit(EXIT_FAILURE);
}

list_type_t get_list_type(size_t bytes) {
    if (bytes <= 32) {
        return BYTES_32;
    } 
    if (bytes <= 64) {
        return BYTES_64;
    }
    if (bytes <= 128) {
        return BYTES_128;
    }
    if (bytes <= 256) {
        return BYTES_256;
    }
    if (bytes <= 512) {
        return BYTES_512;
    }
    return BYTES_LARGE;
}