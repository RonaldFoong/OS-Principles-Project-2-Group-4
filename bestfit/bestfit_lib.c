#include "bestfit_lib.h"
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "../utils/alloc_list.h"
#include "../utils/chunk_size_of.h"

void *alloc(size_t chunk_size) {
    alloc_node_t *best_node = NULL;
    for (alloc_node_t *node = free_chunks.head; node != NULL; node = node->next) {
        if (node->alloc->total_size == chunk_size) {
            best_node = node;
            break;
        }
        if (node->alloc->total_size > chunk_size && (best_node == NULL || node->alloc->total_size < best_node->alloc->total_size)) {
            best_node = node;
        }
    }
    if (best_node != NULL) {
        if (best_node->prev != NULL) {
            best_node->prev->next = best_node->next;
        } else {
            free_chunks.head = best_node->next;
        }

        if (best_node->next != NULL) {
            best_node->next->prev = best_node->prev;
        } else {
            free_chunks.tail = best_node->prev;
        }

        allocation_t *alloc = best_node->alloc;
        alloc->used_size = chunk_size;
        free(best_node);
        if (!push(&allocated_chunks, alloc)) {
            fprintf(stderr, "Error: Could not allocate chunk\n");
            exit(EXIT_FAILURE);
        }

        return alloc->space;
    }

    size_t total_size = chunk_size_of(chunk_size);
    if (total_size == 0) {
        fprintf(stderr, "Error: Chunk size is too large\n");
        exit(EXIT_FAILURE);
    }

    void *space = sbrk(total_size);
    if (space == (void *)-1) {
        fprintf(stderr, "Error: Could not allocate chunk\n");
        exit(EXIT_FAILURE);
    }

    allocation_t *alloc = malloc(sizeof(allocation_t));
    if (alloc == NULL) {
        fprintf(stderr, "Error: Could not allocate memory\n");
        exit(EXIT_FAILURE);
    }

    alloc->total_size = total_size;
    alloc->used_size = chunk_size;
    alloc->space = space;
    if (!push(&allocated_chunks, alloc)) {
        fprintf(stderr, "Error: Could not allocate chunk\n");
        exit(EXIT_FAILURE);
    }

    return space;
}

void dealloc(void *chunk) {
    for (alloc_node_t *node = allocated_chunks.head; node != NULL; node = node->next) {
        if (node->alloc->space == chunk) {
            if (node->prev != NULL) {
                node->prev->next = node->next;
            } else {
                allocated_chunks.head = node->next;
            }

            if (node->next != NULL) {
                node->next->prev = node->prev;
            } else {
                allocated_chunks.tail = node->prev;
            }

            allocation_t *alloc = node->alloc;
            alloc->used_size = 0;
            free(node);
            if (!push(&free_chunks, alloc)) {
                fprintf(stderr, "Error: Could not deallocate chunk\n");
                exit(EXIT_FAILURE);
            }
            return;
        }
    }
    fprintf(stderr, "Error: Chunk does not exist\n");
    exit(EXIT_FAILURE);
}