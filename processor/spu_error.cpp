#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


#include "spu_error.h"
#include "spu_data.h"


const char* error_messages[ERROR_TABLE_SIZE] = {};


void initializeErrorMessages()
{
    error_messages[ERR_OK]                    = "No errors";
    error_messages[ERR_FILE_OPEN]             = "Failed to open input file";
    error_messages[ERR_FILE_READ]             = "Failed to read from file";
    error_messages[ERR_FILE_CLOSE]            = "Failed to close file";
    error_messages[ERR_STACK]                 = "Stack operation failed (possibly corrupted stack)";
    error_messages[ERR_INVALID_CMD_ARGUMENT]  = "Invalid command-line argument";
    error_messages[ERR_INVALID_INSTRUCTION]   = "Invalid instruction in bytecode";
    error_messages[ERR_INVALID_MEMORY_ACCESS] = "Invalid memory access (RAM index out of range)";
    error_messages[ERR_INVALID_ENTER]         = "Invalid enter";
    error_messages[ERR_INVALID_REGISTER]      = "Invalid register index";
    error_messages[ERR_OUT_OF_MEMORY]         = "Out of memory";
    error_messages[ERR_EXIT]                  = "Program terminated successfully (HLT instruction)"; 
}


ErrorCode spuVerify(Processor* spu)
{
    ASSERT_SPU(spu);

    StackError err = stackVerify(&spu->stack);
    if (err != SUCCESS) {
#ifdef DEBUG
        stackDump(&spu->stack, err);
#endif // DEBUG
        return ERR_STACK;
    }

    err = stackVerify(&spu->stack);
    if (err != SUCCESS) {
#ifdef DEBUG
        stackDump(&spu->stack, err);
#endif // DEBUG
        return ERR_STACK;
    }

    return ERR_OK;
}


void printError(ErrorCode err)
{
    fprintf(stderr, "%s\n", error_messages[err]);
}
