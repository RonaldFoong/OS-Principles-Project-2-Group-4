#include "alloc_list.h"

#include <stdlib.h>

bool list_append(alloc_list_t *list, allocation_t *allocation)
{
    alloc_node_t *node = malloc(sizeof(*node));

    if (node == NULL) {
        return false;
    }
    node->allocation = allocation;
    node->next = NULL;
    node->prev = list->tail;

    if (list->tail != NULL) {
        list->tail->next = node;
    } else {
        list->head = node;
    }
    list->tail = node;
    list->length++;
    return true;
}

allocation_t *list_remove(alloc_list_t *list, alloc_node_t *node)
{
    allocation_t *allocation;

    if (node == NULL) {
        return NULL;
    }
    if (node->prev != NULL) {
        node->prev->next = node->next;
    } else {
        list->head = node->next;
    }
    if (node->next != NULL) {
        node->next->prev = node->prev;
    } else {
        list->tail = node->prev;
    }
    allocation = node->allocation;
    free(node);
    list->length--;
    return allocation;
}

allocation_t *list_pop_back(alloc_list_t *list)
{
    return list_remove(list, list->tail);
}

void list_destroy(alloc_list_t *list, bool free_allocations)
{
    alloc_node_t *node = list->head;

    while (node != NULL) {
        alloc_node_t *next = node->next;

        if (free_allocations) {
            free(node->allocation);
        }
        free(node);
        node = next;
    }
    list->head = NULL;
    list->tail = NULL;
    list->length = 0U;
}