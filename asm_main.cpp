#include <stdio.h>

#include "asm_data.h"
#include "asm.h"
#include "asm_input.h"
#include "asm_output.h"


int main(int argc, const char** argv)
{
    AssemblyData data = {};
    ErrorCode error_code = ERR_OK;

    error_code = parseArguments(&data.args, (size_t)argc, argv);
    REPORT_AND_RETURN(error_code, &data);

    printf("MEOW1\n");
    error_code = initializeBuffer(&data.buffer, data.args.input_file);
    REPORT_AND_RETURN(error_code, &data);

    printf("MEOW2\n");
    error_code = assembler(&data.code, data.buffer);
    REPORT_AND_RETURN(error_code, &data);

    printf("MEOW3\n");
    error_code = writeByteCodeToFile(&data.code, data.args.output_file);
    REPORT_AND_RETURN(error_code, &data);
  
    printf("MEOWFINAL\n");
    printf("%zu\n", data.code.instruction_count);
    destroyData(&data);

    return 0;
}
