#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "spu_input.h"


ErrorCode loadByteCode(ByteCode* code, const char* input_filename)
{
    assert(code != NULL);
    assert(input_filename != NULL);

    FILE* in = fopen(input_filename, "r");
    if (in == NULL)
        return ERR_FILE_OPEN;

    if (fscanf(in, "%zu", &code->instruction_count) != 1 ||
        code->instruction_count == 0) {
        fclose(in); 
        return ERR_FILE_READ;
    }

    ErrorCode err = ERR_OK;
    err = createBuffer(code, code->instruction_count);
    if (err != ERR_OK)
        return ERR_OUT_OF_MEMORY;

    err = readInstructions(code, in);
    if (fclose(in) != 0)
        return ERR_FILE_CLOSE;

    return err;
}


ErrorCode createBuffer(ByteCode* code, size_t size)
{
    assert(code != NULL);

    code->buffer = (int*)calloc(size, sizeof(int));  
    if (code->buffer == NULL)
        return ERR_OUT_OF_MEMORY;

    return ERR_OK;
}


ErrorCode readInstructions(ByteCode* code, FILE* in)
{
    assert(code != NULL);
    assert(code->buffer != NULL);
    assert(in != NULL);

    for (size_t index = 0; index < code->instruction_count; index++) {
        int value = 0;
        if (fscanf(in, "%d", &value) != 1)
            return ERR_FILE_READ;

        code->buffer[index] = value;
    }
    
    return ERR_OK;
}


ErrorCode parseArguments(Arguments* args, size_t argc, const char** argv)
{
    assert(args != NULL);
    assert(argv != NULL);

    size_t index = 1;
    for (; index < argc; index++) {
        if (strcmp(argv[index], "-i") == 0)
            parseInputFile(argc, argv, args, &index);
        else
            return ERR_INVALID_CMD_ARGUMENT;
    }

    return ERR_OK;
}


void parseInputFile(size_t argc, const char** argv, Arguments* args, size_t* index)
{
    assert(argv != NULL);
    assert(args != NULL);
    assert(index != NULL);

    if (*index < argc - 1)
        args->input_file = argv[++(*index)];
}
