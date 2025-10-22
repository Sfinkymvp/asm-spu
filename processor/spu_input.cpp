#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


#include "spu_input.h"


ErrorCode loadNumericByteCode(Processor* spu)
{
    assert(spu != NULL);
    assert(spu->args.input_file != NULL);

    FILE* in = fopen(spu->args.input_file, "r");
    if (in == NULL)
        return ERR_FILE_OPEN;

    if (fscanf(in, "%zu", &spu->bytecode.capacity) != 1 ||
        spu->bytecode.capacity == 0) {
        fclose(in); 
        return ERR_FILE_READ;
    }

    ErrorCode err = createBuffer(spu);
    if (err != ERR_OK) {
        fclose(in);
        return err;
    }

    err = readByteCode(spu, in);
    if (fclose(in) != 0)
        return err;

    return err;
}


ErrorCode loadBinaryByteCode(Processor* spu)
{
    assert(spu != NULL);
    assert(spu->args.input_file != NULL);

    FILE* in = fopen(spu->args.input_file, "rb");
    if (in == NULL)
        return ERR_FILE_OPEN;

    if (fread(&spu->bytecode.capacity, sizeof(int), 1, in) != 1) {
        fclose(in);
        return ERR_FILE_READ;
    }

    ErrorCode err = createBuffer(spu);
    if (err != ERR_OK) {
        fclose(in);
        return err;
    }

    if (fread(spu->bytecode.data, sizeof(int), spu->bytecode.capacity, in) != 
        spu->bytecode.capacity) {
        fclose(in);
        return ERR_FILE_READ;
    }

    if (fclose(in) != 0)
        return ERR_FILE_CLOSE;

    return ERR_OK;
}


ErrorCode createBuffer(Processor* spu)
{
    assert(spu != NULL);

    spu->bytecode.data = (int*)calloc(spu->bytecode.capacity, sizeof(int));  
    if (spu->bytecode.data == NULL)
        return ERR_OUT_OF_MEMORY;

    return ERR_OK;
}


ErrorCode readByteCode(Processor* spu, FILE* in)
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

    CmdArgs args = {argc, argv, 1};
    spu->args.is_numeric_input = false;

    for (; args.pos < args.count; args.pos++) {
        if (strcmp(args.values[args.pos], "-i") == 0)
            parseInputFile(spu, &args);
        else if (strcmp(args.values[args.pos], "--numeric") == 0)
            spu->args.is_numeric_input = true;
        else
            return ERR_INVALID_CMD_ARGUMENT;
    }

    return ERR_OK;
}


void parseInputFile(Processor* spu, CmdArgs* args)
{
    assert(spu != NULL);
    assert(args != NULL);
    assert(args->values != NULL);

    if (args->pos < args->count - 1)
        spu->args.input_file = args->values[++args->pos];
}
