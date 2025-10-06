#include <stdio.h>
#include <assert.h>

#include "asm_output.h"
#include "asm_data.h"


ErrorCode writeByteCodeToFile(const ByteCode* code, const char* output_filename)
{
    assert(code != NULL);
    assert(output_filename != NULL);

    FILE* out = fopen(output_filename, "w");
    if (out == NULL)
        return ERR_FILE_OPEN;

    if (fprintf(out, "%zu", code->instruction_count) < 0) {
        fclose(out);
        return ERR_FILE_WRITE;
    }
    for (size_t index = 0; index < code->instruction_count; index++)
        if (fprintf(out, " %d", code->data[index]) < 0) {
            fclose(out);
            return ERR_FILE_WRITE; 
        }

    if (fclose(out) != 0)
        return ERR_FILE_CLOSE;

    return ERR_OK;
}


void printError(ErrorCode error_code)
{
    fprintf(stderr, "%s\n", error_messages[error_code]);
}

