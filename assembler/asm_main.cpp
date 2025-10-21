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
    RETURN_IF_ERROR(err, &asmdata);

    err = assembler(&asmdata);
    printf("%d\n", asmdata.ip);
    RETURN_IF_ERROR(err, &asmdata);

    err = writeByteCodeToFile(&asmdata);
    RETURN_IF_ERROR(err, &asmdata);
  
    asmDtor(&asmdata);
    return 0;
}
