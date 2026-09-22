#include "print_list.h"
#include <stdio.h>
#include "alloc_list.h"

void print_alloc_list_contents(alloc_list_t *list) {
    alloc_node_t *node = list->head;
    if (node != NULL) {
        printf(
            "{addr: %p, total size: %zu, used size: %zu}", 
            node->alloc->space, 
            node->alloc->total_size, 
            node->alloc->used_size
        );
        node = node->next;
    } else {
        printf("{}");
    }
    for (; node != NULL; node = node->next) {
        printf(
            " -> {addr: %p, total size: %zu, used size: %zu}", 
            node->alloc->space, 
            node->alloc->total_size, 
            node->alloc->used_size
        );
    }
    printf("\n");
}

void print_free_list_contents(alloc_list_t *list) {
    alloc_node_t *node = list->head;
    if (node != NULL) {
        printf(
            "{addr: %p, total size: %zu}", 
            node->alloc->space, 
            node->alloc->total_size
        );
        node = node->next;
    } else {
        printf("{}");
    }
    for (; node != NULL; node = node->next) {
        printf(
            " -> {addr: %p, total size: %zu}", 
            node->alloc->space, 
            node->alloc->total_size
        );
    }
    printf("\n");
}