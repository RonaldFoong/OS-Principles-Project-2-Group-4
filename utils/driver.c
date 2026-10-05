#include <stdio.h>
#include <stdlib.h>
#include "driver.h"
#include "mem_common.h"
#include <string.h>

#define MAX_CHUNKS 10000

int run_simulation(int argc, char *argv[], const char *strategy_name){

    if(argc != 2)
    {
        fprintf(stderr, "Argument count incorrect,\nplease make sure to have 2 arguments \nand make sure to include datafile, not just %s\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE *file = fopen(argv[1], "r");

    if(file == NULL)
    {
        fprintf(stderr, "Error: cannot open %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    //printf("File opened successfully\n");

    static void* stack[MAX_CHUNKS];
    int count = 0;
    char line[100];
    size_t size;
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if(sscanf(line, "alloc: %zu", &size)==1)
        {
            //printf("Allocation request: %zu\n", size);

            if(count == MAX_CHUNKS)
            {
                fprintf(stderr, "Error: too many chunks in use\n");
                break;
            }

            void* chunk = alloc(size);

            if(chunk != NULL)
            {
                stack[count] = chunk;
                count++;
                //printf("Active allocations: %d\n", count);
            }
        }
        else if(strncmp(line, "dealloc", strlen("dealloc")) == 0)
        {
            //printf("Deallocation request\n");
            
            if(count > 0)
            {
                count--;
                dealloc(stack[count]);
               // printf("Active allocations: %d\n", count);
            }
            else{
                fprintf(stderr, "Warning: dealloc with nothing allocated\n");
            }
        }
        else
        {
           // printf("Not an allocation: %s", line);
        }
    }

    fclose(file);

    printf("=== %s ===\n", strategy_name);
    print_memory();
    free_records();

    return EXIT_SUCCESS;
}