#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


#include "asm_data.h"
#include "asm_io.h"
#include "asm_commands.h"


CommandInfo commands[] = {
    {CMD_HLT,   "HLT",   0, asmCmdNoArgs},
    {CMD_PUSH,  "PUSH",  0, asmCmdPush},
    {CMD_POPR,  "POP",   0, asmCmdPop},
    {CMD_ADD,   "ADD",   0, asmCmdNoArgs},
    {CMD_SUB,   "SUB",   0, asmCmdNoArgs},
    {CMD_DIV,   "DIV",   0, asmCmdNoArgs},
    {CMD_MUL,   "MUL",   0, asmCmdNoArgs},
    {CMD_SQRT,  "SQRT",  0, asmCmdNoArgs},
    {CMD_IN,    "IN",    0, asmCmdNoArgs},
    {CMD_OUT,   "OUT",   0, asmCmdNoArgs},
    {CMD_JMP,   "JMP",   0, asmCmdJump},
    {CMD_JB,    "JB",    0, asmCmdJump},
    {CMD_JBE,   "JBE",   0, asmCmdJump},
    {CMD_JA,    "JA",    0, asmCmdJump},
    {CMD_JAE,   "JAE",   0, asmCmdJump},
    {CMD_JE,    "JE",    0, asmCmdJump},
    {CMD_JNE,   "JNE",   0, asmCmdJump},
    {CMD_CALL,  "CALL",  0, asmCmdJump},
    {CMD_RET,   "RET",   0, asmCmdNoArgs},
    {CMD_DRAW,  "DRAW",  0, asmCmdOneArg}
};


const size_t COMMAND_COUNT = sizeof(commands) / sizeof(*commands);


RegisterInfo regs[] = {
    {RESERVED, "RESERVED",   0},
    {RAX,      "RAX",        0},
    {RBX,      "RBX",        0},
    {RCX,      "RCX",        0},
    {RDX,      "RDX",        0},
    {REX,      "REX",        0},
    {RFX,      "RFX",        0},
    {RGX,      "RGX",        0}
};


RegisterInfo memory_regs[] = {
    {RESERVED, "[RESERVED]", 0},
    {RAX,      "[RAX]",      0},
    {RBX,      "[RBX]",      0},
    {RCX,      "[RCX]",      0},
    {RDX,      "[RDX]",      0},
    {REX,      "[REX]",      0},
    {RFX,      "[RFX]",      0},
    {RGX,      "[RGX]",      0}
};


const char* error_messages[ERROR_TABLE_SIZE] = {};


void initializeErrorMessages()
{
    error_messages[ERR_OK]                   = "No errors";
    error_messages[ERR_FILE_OPEN]            = "Failed to open source file";
    error_messages[ERR_FILE_READ]            = "Failed to read from source file";
    error_messages[ERR_FILE_WRITE]           = "Failed to write output file";
    error_messages[ERR_FILE_CLOSE]           = "Failed to close file";
    error_messages[ERR_INVALID_CMD_ARGUMENT] = "Invalid command-line argument";
    error_messages[ERR_INVALID_INSTRUCTION]  = "Unknown or invalid instruction name";
    error_messages[ERR_INVALID_REGISTER]     = "Invalid register name or index";
    error_messages[ERR_INVALID_LABEL]        = "Undefined or invalid label reference";
    error_messages[ERR_INVALID_OPERAND]      = "Invalid or unexpected operand format";
    error_messages[ERR_OUT_OF_MEMORY]        = "Memory allocation failed (out of memory)";
}


void printError(ErrorCode err)
{
    fprintf(stderr, "%s\n", error_messages[err]);
}


ErrorCode initializeByteCode(AssemblyData* asmdata, size_t start_capacity)
{
    assert(asmdata != NULL);

    asmdata->code.data = (int*)calloc(start_capacity, sizeof(int));
    if (asmdata->code.data == NULL)
        return ERR_OUT_OF_MEMORY;

    asmdata->code.capacity = start_capacity;
    return ERR_OK;
}


ErrorCode expandByteCode(AssemblyData* asmdata)
{
    ASSERT_ASM(asmdata);

    void* temp = realloc(asmdata->code.data, asmdata->code.capacity * 2 * sizeof(int));
    if (temp == NULL)
        return ERR_OUT_OF_MEMORY;

    asmdata->code.data = (int*)temp;
    asmdata->code.capacity *= 2;
    return ERR_OK;
}


ErrorCode initializeLabelTables(AssemblyData* asmdata, size_t start_capacity)
{
    assert(asmdata != NULL);

    asmdata->label_table.labels = (Label*)calloc(start_capacity, sizeof(Label));
    if (asmdata->label_table.labels == NULL)
        return ERR_OUT_OF_MEMORY;
    asmdata->label_table.capacity = start_capacity;

    asmdata->fixups.labels = (Label*)calloc(start_capacity, sizeof(Label));
    if (asmdata->fixups.labels == NULL)
        return ERR_OUT_OF_MEMORY;
    asmdata->fixups.capacity = start_capacity;

    return ERR_OK;
}


ErrorCode ExpandLabelTable(AssemblyData* asmdata)
{
    ASSERT_ASM(asmdata);

    printf("size of label table: %zu\n", asmdata->label_table.capacity);
    void* temp = realloc(asmdata->label_table.labels, 2 * asmdata->label_table.capacity * sizeof(Label));
    if (temp == NULL)
        return ERR_OUT_OF_MEMORY;

    asmdata->label_table.labels = (Label*)temp;
    asmdata->label_table.capacity *= 2;
    return ERR_OK;
}


ErrorCode asmCtor(AssemblyData* asmdata, int argc, const char** argv)
{
    assert(asmdata != NULL);
    assert(argv != NULL);

    CHECK_OK(parseArguments(asmdata, argc, argv));
    CHECK_OK(initializeBuffer(asmdata));
    CHECK_OK(initializeByteCode(asmdata, START_CAPACITY));
    CHECK_OK(initializeLabelTables(asmdata, START_CAPACITY));

    return ERR_OK;
}


void asmDtor(AssemblyData* asmdata) 
{
    assert(asmdata != NULL);

    free(asmdata->buffer);
    free(asmdata->code.data);
    free(asmdata->label_table.labels);
    free(asmdata->fixups.labels);
}
