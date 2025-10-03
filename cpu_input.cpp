#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "cpu_input.h"


bool input(Program* program, const char* input_filename)
{
    assert(program != NULL);
    assert(input_filename != NULL);

    FILE* in = fopen(input_filename, "r");
    if (in == NULL)
        return false;

    int value = 0;
    while (fscanf(in, "%d", &value) == 1) {
        if (program->data == NULL) {
            program->data = (int*)calloc(8, sizeof(int));
            program->capacity = 8;
        }
        if (program->size == program->capacity) {
            program->data = (int*)realloc(program->data, program->capacity * 2 * sizeof(int));
            program->capacity *= 2;
        }
        program->data[program->size++] = value;
    }

    fclose(in);
    return true;
}
