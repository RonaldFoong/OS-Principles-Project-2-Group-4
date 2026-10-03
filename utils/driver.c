#include "driver.h"

#include "allocator_api.h"

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct pointer_stack {
    void **items;
    size_t length;
    size_t capacity;
} pointer_stack_t;

static void stack_push(pointer_stack_t *stack, void *pointer)
{
    if (stack->length == stack->capacity) {
        size_t new_capacity = stack->capacity == 0U ? 16U : stack->capacity * 2U;
        void **new_items = realloc(stack->items,
                                   new_capacity * sizeof(*new_items));

        if (new_items == NULL) {
            fprintf(stderr, "Error: unable to grow the test pointer stack.\n");
            exit(EXIT_FAILURE);
        }
        stack->items = new_items;
        stack->capacity = new_capacity;
    }
    stack->items[stack->length++] = pointer;
}

static char *skip_space(char *text)
{
    while (isspace((unsigned char)*text)) {
        text++;
    }
    return text;
}

// Returns 1 for alloc, 0 for another command, and -1 for malformed alloc
static int parse_alloc_line(char *line, size_t *size)
{
    char *cursor = skip_space(line);
    char *end;
    unsigned long value;

    if (strncmp(cursor, "alloc", 5U) != 0) {
        return 0;
    }
    cursor = skip_space(cursor + 5);
    if (*cursor != ':') {
        return -1;
    }
    cursor = skip_space(cursor + 1);
    if (*cursor == '-') {
        return -1;
    }
    errno = 0;
    value = strtoul(cursor, &end, 10);
    if (errno != 0 || end == cursor) {
        return -1;
    }
    end = skip_space(end);
    if (*end != '\0') {
        return -1;
    }
    *size = (size_t)value;
    if ((unsigned long)*size != value) {
        return -1;
    }
    return 1;
}

static int is_dealloc_line(char *line)
{
    char *cursor = skip_space(line);
    char *end;

    if (strncmp(cursor, "dealloc", 7U) != 0) {
        return 0;
    }
    end = skip_space(cursor + 7);
    return *end == '\0';
}

int run_allocator_program(int argc, char **argv)
{
    FILE *input;
    pointer_stack_t stack = {NULL, 0U, 0U};
    char line[256];
    size_t line_number = 0U;
    int status = EXIT_SUCCESS;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s datafile\n", argv[0]);
        return EXIT_FAILURE;
    }
    input = fopen(argv[1], "r");
    if (input == NULL) {
        fprintf(stderr, "Error: cannot open '%s'.\n", argv[1]);
        return EXIT_FAILURE;
    }

    while (fgets(line, sizeof(line), input) != NULL) {
        size_t size;
        size_t length;
        int parse_result;

        line_number++;
        length = strlen(line);
        if (length > 0U && line[length - 1U] == '\n') {
            line[--length] = '\0';
        } else if (!feof(input)) {
            fprintf(stderr, "Error: line %zu is too long.\n", line_number);
            status = EXIT_FAILURE;
            break;
        }
        if (length > 0U && line[length - 1U] == '\r') {
            line[length - 1U] = '\0';
        }

        parse_result = parse_alloc_line(line, &size);
        if (parse_result == 1) {
            if (size == 0U || size > 512U) {
                fprintf(stderr,
                        "Error: line %zu requests %zu bytes; valid sizes are 1-512.\n",
                        line_number, size);
                status = EXIT_FAILURE;
                break;
            }
            stack_push(&stack, alloc(size));
        } else if (is_dealloc_line(line)) {
            if (stack.length == 0U) {
                fprintf(stderr,
                        "Error: line %zu deallocates when no chunk is allocated.\n",
                        line_number);
                status = EXIT_FAILURE;
                break;
            }
            dealloc(stack.items[--stack.length]);
        } else if (*skip_space(line) != '\0') {
            fprintf(stderr, "Error: invalid command on line %zu.\n", line_number);
            status = EXIT_FAILURE;
            break;
        }
    }

    if (ferror(input)) {
        fprintf(stderr, "Error: failed while reading '%s'.\n", argv[1]);
        status = EXIT_FAILURE;
    }
    if (fclose(input) != 0) {
        fprintf(stderr, "Error: failed to close '%s'.\n", argv[1]);
        status = EXIT_FAILURE;
    }
    if (status == EXIT_SUCCESS) {
        allocator_print_state();
    }
    free(stack.items);
    allocator_destroy();
    return status;
}
