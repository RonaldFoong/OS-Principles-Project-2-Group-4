#ifndef PRINT_LIST_H
#define PRINT_LIST_H

#include <stdio.h>
#include "alloc_list.h"

void print_alloc_list_contents(alloc_list_t *list);
void print_free_list_contents(alloc_list_t *list);

#endif