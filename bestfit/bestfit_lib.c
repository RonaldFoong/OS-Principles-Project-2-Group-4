#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "../utils/alloc_list.h"
#include "../utils/chunk_size_of.h"
#include "bestfit_lib.h"

alloc_list_t allocated_chunks;
alloc_list_t free_chunks;

void *alloc(size_t chunk_size) {
    allocation_t *alloc = NULL;
    size_t alloc_chunk_size = 999;
    alloc_node_t *node = NULL;
    for (node = free_chunks.head; node != NULL; node = node->next) {
        if (node->alloc->total_size == chunk_size) {
            alloc = node->alloc;
            alloc->used_size = chunk_size;
            alloc_chunk_size = chunk_size;
            break;
        }
        if (node->alloc->total_size > chunk_size && node->alloc->total_size < alloc_chunk_size) {
            alloc = node->alloc;
            alloc_chunk_size = node->alloc->total_size;
        }
    }
    if (alloc != NULL) {
        if (node->prev != NULL && node->next != NULL) {
            node->prev->next = node->next;
            node->next->prev = node->prev;
        } else if (node->prev != NULL) {
            node->prev->next = NULL;
        } else if (node->next != NULL) {
            node->next->prev = NULL;
        }

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

    alloc = malloc(sizeof(allocation_t));
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
            if (!push(&free_chunks, alloc)) {
                fprintf(stderr, "Error: Could not deallocate chunk");
                exit(EXIT_FAILURE);
            }
            return;
        }
    }
    fprintf(stderr, "Error: Chunk does not exist");
    exit(EXIT_FAILURE);
}