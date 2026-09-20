#ifndef ALLOC_LIST_H
#define ALLOC_LIST_H

typedef struct {
    size_t size;
    void *space;
} allocation_t;

typedef struct {
    allocation_t *head;
    allocation_t *tail;
} alloc_list_t;

void push(alloc_list_t *self, allocation_t *alloc);
void pop(alloc_list_t *self, allocation_t *alloc);

#endif