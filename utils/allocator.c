#include "allocator.h"
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 128
#define STACK_SIZE 4096

// alloc and dealloc implemented in the 3 main programs
void *alloc(size_t chunk_size);
void dealloc(void *chunk);

int allocator(int argc, char **argv, const char *program_name) {
    // Validate CLI args
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <datafile>\n", program_name);
        return 1;
    }

    // Open datafile
    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: Could not open file %s\n", argv[1]);
        return 1;
    }

    // Parse datafile and call alloc and dealloc
    void *alloc_stack[STACK_SIZE];
    int sp = 0;
    char buffer[BUFFER_SIZE];
    while (fgets(buffer, BUFFER_SIZE, fp) != NULL) {
        char call[BUFFER_SIZE];
        size_t chunk_size = 0;

        int items_assigned = sscanf(buffer, "%s %zu", call, &chunk_size);
        if (items_assigned == 1 && strcmp(call, "dealloc") == 0) {
            if (sp <= 0) {
                fprintf(stderr, "Error: Allocation stack is empty\n");
                fclose(fp);
                return 1;
            }
            dealloc(alloc_stack[--sp]);
        } else if (items_assigned == 2 && strcmp(call, "alloc:") == 0) {
            if (sp >= STACK_SIZE) {
                fprintf(stderr, "Error: Allocation stack is full\n");
                fclose(fp);
                return 1;
            }
            alloc_stack[sp++] = alloc(chunk_size);
        } else {
            fprintf(stderr, "Error: Could not read file %s\n", argv[1]);
            fclose(fp);
            return 1;
        }
    }
    if (ferror(fp)) {
        fprintf(stderr, "Error: Could not read file %s\n", argv[1]);
        fclose(fp);
        return 1;
    }

    fclose(fp);
    return 0;
}