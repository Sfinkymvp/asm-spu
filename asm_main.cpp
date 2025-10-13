#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "asm_data.h"
#include "asm.h"
#include "asm_input.h"
#include "asm_output.h"


int main(int argc, const char** argv)
{
    AssemblyData asmdata = {};
    ErrorCode err = ERR_OK;

    err = parseArguments(&asmdata.args, (size_t)argc, argv);
    REPORT_AND_RETURN(err, &asmdata);

    printf("MEOW1\n");
    err = initializeBuffer(&asmdata.buffer, asmdata.args.input_file);
    REPORT_AND_RETURN(err, &asmdata);

    asmdata.label_table.labels = (Label*)calloc(START_CAPACITY, sizeof(Label));
    asmdata.label_table.capacity = START_CAPACITY;
    asmdata.refs_table.labels = (Label*)calloc(START_CAPACITY, sizeof(Label));
    asmdata.refs_table.capacity = START_CAPACITY;

    assert(asmdata.label_table.labels != NULL);
    assert(asmdata.refs_table.labels != NULL);

    printf("MEOW2\n");
    err = assembler(&asmdata);
    REPORT_AND_RETURN(err, &asmdata);

    printf("MEOW3\n");
    err = writeByteCodeToFile(&asmdata.code, asmdata.args.output_file);
    REPORT_AND_RETURN(err, &asmdata);
  
    printf("MEOWFINAL\n");
    printf("%zu\n", asmdata.code.instruction_count);
    assemblerDtor(&asmdata);

    return 0;
}
