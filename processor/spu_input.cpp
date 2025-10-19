#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "spu_input.h"


ErrorCode loadByteCode(Processor* spu, const char* input_filename)
{
    assert(spu != NULL);
    assert(input_filename != NULL);

    FILE* in = fopen(input_filename, "r");
    if (in == NULL)
        return ERR_FILE_OPEN;

    if (fscanf(in, "%zu", &spu->bytecode.capacity) != 1 ||
        spu->bytecode.capacity == 0) {
        fclose(in); 
        return ERR_FILE_READ;
    }

    ErrorCode err = ERR_OK;
    err = createBuffer(spu);
    if (err != ERR_OK)
        return ERR_OUT_OF_MEMORY;

    err = readInstructions(spu, in);
    if (fclose(in) != 0)
        return ERR_FILE_CLOSE;

    return err;
}


ErrorCode createBuffer(Processor* spu)
{
    assert(spu != NULL);

    spu->bytecode.data = (int*)calloc(spu->bytecode.capacity, sizeof(int));  
    if (spu->bytecode.data == NULL)
        return ERR_OUT_OF_MEMORY;

    return ERR_OK;
}


ErrorCode readInstructions(Processor* spu, FILE* in)
{
    assert(spu != NULL);
    assert(in != NULL);

    for (size_t index = 0; index < spu->bytecode.capacity; index++) {
        int value = 0;
        if (fscanf(in, "%d", &value) != 1)
            return ERR_FILE_READ;

        spu->bytecode.data[index] = value;
    }
    
    return ERR_OK;
}


ErrorCode parseArguments(Processor* spu, int argc, const char** argv)
{
    assert(spu != NULL);
    assert(argv != NULL);

    int index = 1;
    for (; index < argc; index++) {
        if (strcmp(argv[index], "-i") == 0)
            parseInputFile(argc, argv, spu, &index);
        else
            return ERR_INVALID_CMD_ARGUMENT;
    }

    return ERR_OK;
}


void parseInputFile(int argc, const char** argv, Processor* spu, int* index)
{
    assert(argv != NULL);
    assert(spu != NULL);
    assert(index != NULL);

    if (*index < argc - 1)
        spu->args.input_file = argv[++(*index)];
}
