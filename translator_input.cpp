#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <assert.h>

#include "translator_input.h"


size_t getFileSize(const char* input_filename)
{
    assert(input_filename != NULL);

    struct stat buf = {};

    if (stat(input_filename, &buf) == -1)
        return 0;

    return (size_t)buf.st_size;
}


char* initializeBuffer(const char* input_filename)
{
    assert(input_filename != NULL);

    FILE* in = fopen(input_filename, "r");
    if (in == NULL) {
        return NULL;
    }

    size_t size = getFileSize(input_filename);
    if (size == 0)
        return NULL;

    char* buffer  = (char*)calloc(size, 1);
    if (buffer == NULL)
        return NULL;

    fread(buffer, 1, size - 1, in);
    buffer[size - 1] = '\0';

    if (fclose(in) != 0) {
        free(buffer);
        buffer = NULL;
    }

    return buffer;
}
