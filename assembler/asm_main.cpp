#include <stdio.h>
#include <assert.h>


#include "asm_data.h"
#include "asm.h"
#include "asm_commands.h"
#include "asm_io.h"
#include "asm_labels.h"
#include "asm_hash.h"


int main(int argc, const char** argv)
{
    initializeHashes();
    initializeErrorMessages();

    AssemblyData asmdata = {};
    ErrorCode err = ERR_OK;

    err = asmCtor(&asmdata, argc, argv);
    REPORT_AND_RETURN(err, &asmdata);

    printf("MEOWONE\n");
    err = assembler(&asmdata);
    REPORT_AND_RETURN(err, &asmdata);

    printf("MEOWTWO\n");
    err = writeByteCodeToFile(&asmdata);
    REPORT_AND_RETURN(err, &asmdata);
  
    printf("MEOWFINAL\n");

    asmDtor(&asmdata);
    return 0;
}
