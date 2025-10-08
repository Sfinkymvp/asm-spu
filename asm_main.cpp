#include <stdio.h>

#include "asm_data.h"
#include "asm.h"
#include "asm_input.h"
#include "asm_output.h"


int main(int argc, const char** argv)
{
    AssemblyData data = {};
    ErrorCode err = ERR_OK;

    err = parseArguments(&data.args, (size_t)argc, argv);
    REPORT_AND_RETURN(err, &data);

    printf("MEOW1\n");
    err = initializeBuffer(&data.buffer, data.args.input_file);
    REPORT_AND_RETURN(err, &data);

    printf("MEOW2\n");
    err = assembler(&data.code, data.buffer);
    REPORT_AND_RETURN(err, &data);

    printf("MEOW3\n");
    err = writeByteCodeToFile(&data.code, data.args.output_file);
    REPORT_AND_RETURN(err, &data);
  
    printf("MEOWFINAL\n");
    printf("%zu\n", data.code.instruction_count);
    destroyData(&data);

    return 0;
}
