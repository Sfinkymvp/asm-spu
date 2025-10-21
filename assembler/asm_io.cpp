#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <sys/stat.h>


#include "asm_io.h"
#include "asm_data.h"


size_t getFileSize(const char* input_filename)
{
    assert(input_filename != NULL);

    struct stat buf = {};

    if (stat(input_filename, &buf) == -1)
        return 0;

    return (size_t)buf.st_size;
}


ErrorCode writeByteCodeToFile(const AssemblyData* asmdata)
{
    assert(asmdata != NULL);
    assert(asmdata->args.output_file != NULL);

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


ErrorCode parseArguments(AssemblyData* asmdata, int argc, const char** argv)
{
    assert(asmdata != NULL);
    assert(argv != NULL);

    CmdArgs args = {argc, argv, 1};
    for (; args.pos < argc; args.pos++) {
        if (strcmp(args.values[args.pos], "-i") == 0)
            parseInputFile(asmdata, &args);
        else if (strcmp(args.values[args.pos], "-o") == 0)
            parseOutputFile(asmdata, &args);
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
