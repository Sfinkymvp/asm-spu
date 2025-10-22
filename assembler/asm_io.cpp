#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>


#include "asm_io.h"
#include "asm_data.h"


ErrorCode initializeBuffer(AssemblyData* asmdata)
{
    assert(asmdata != NULL);

    FILE* in = fopen(asmdata->args.input_file, "r");
    if (in == NULL)
        return ERR_FILE_OPEN;

    size_t size = getFileSize(asmdata->args.input_file);
    asmdata->buffer = (char*)calloc(size, 1);
    if (asmdata->buffer == NULL)
        return ERR_OUT_OF_MEMORY;

    if (fread(asmdata->buffer, 1, size - 1, in) < size - 1)
        return ERR_FILE_READ;
    asmdata->buffer[size - 1] = '\0';

    if (fclose(in) != 0)
        return ERR_FILE_CLOSE;

    return ERR_OK;
}


size_t getFileSize(const char* input_filename)
{
    assert(input_filename != NULL);

    struct stat buf = {};

    if (stat(input_filename, &buf) == -1)
        return 0;

    return (size_t)buf.st_size;
}


ErrorCode writeNumericByteCode(const AssemblyData* asmdata)
{
    ASSERT_ASM(asmdata);

    FILE* out = fopen(asmdata->args.output_file, "w");
    if (out == NULL)
        return ERR_FILE_OPEN;

    if (fprintf(out, "%d", asmdata->ip) < 0) {
        fclose(out);
        return ERR_FILE_WRITE;
    }
    for (int index = 0; index < asmdata->ip; index++)
        if (fprintf(out, " %d", asmdata->code.data[index]) < 0) {
            fclose(out);
            return ERR_FILE_WRITE; 
        }

    if (fclose(out) != 0)
        return ERR_FILE_CLOSE;

    return ERR_OK;
}


ErrorCode writeBinaryByteCode(const AssemblyData* asmdata)
{
    ASSERT_ASM(asmdata);

    FILE* out = fopen(asmdata->args.output_file, "wb");
    if (out == NULL)
        return ERR_FILE_OPEN;

    if (fwrite(&asmdata->ip, sizeof(int), 1, out) != 1) {
        fclose(out);
        return ERR_FILE_WRITE;
    }

    if (fwrite(asmdata->code.data, sizeof(int), (size_t)asmdata->ip, out)
        != (size_t)asmdata->ip) {
        fclose(out);
        return ERR_FILE_WRITE;
    }

    if (fclose(out) != 0)
        return ERR_FILE_CLOSE;

    return ERR_OK;
}


ErrorCode parseArguments(AssemblyData* asmdata, int argc, const char** argv)
{
    assert(asmdata != NULL);
    assert(argv != NULL);

    CmdArgs args = {argc, argv, 1};
    asmdata->args.is_numeric_output = false;

    for (; args.pos < argc; args.pos++) {
        if (strcmp(args.values[args.pos], "-i") == 0)
            parseInputFile(asmdata, &args);
        else if (strcmp(args.values[args.pos], "-o") == 0)
            parseOutputFile(asmdata, &args);
        else if (strcmp(args.values[args.pos], "--numeric") == 0)
            asmdata->args.is_numeric_output = true;
        else
            return ERR_INVALID_CMD_ARGUMENT;
    }

    return ERR_OK;
}


void parseInputFile(AssemblyData* asmdata, CmdArgs* args)
{
    assert(asmdata != NULL);
    assert(args != NULL);
    assert(args->values != NULL);

    if (args->pos < args->count - 1)
        asmdata->args.input_file = args->values[++args->pos];
}


void parseOutputFile(AssemblyData* asmdata, CmdArgs* args)
{
    assert(asmdata != NULL);
    assert(args != NULL);
    assert(args->values != NULL);

    if (args->pos < args->count - 1)
        asmdata->args.output_file = args->values[++args->pos];
}
