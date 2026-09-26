#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bestfit_lib.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s datafile\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        fprintf(stderr, "Cannot open file: %s\n", argv[1]);
        return 1;
    }

    void *stack[1000];
    int top = 0;

    char line[100];
    while (fgets(line, sizeof(line), file) != NULL) {
        if (strncmp(line, "alloc:", 6) == 0) {
            size_t size = atoi(line + 6);
            stack[top++] = alloc(size);
        } else if (strncmp(line, "dealloc", 7) == 0) {
            if (top > 0)
                dealloc(stack[--top]);
        }
    }

    fclose(file);

    print_lists();
    return 0;
}
