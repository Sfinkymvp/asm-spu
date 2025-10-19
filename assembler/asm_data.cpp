#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


#include "asm_data.h"
#include "asm_io.h"
#include "asm_commands.h"


const char* error_messages[] = {
    "This message should not be printed",
    "This message should not be printed",
    "Failed to open file",
    "Failed to read data from file",
    "Failed to write data to file",
    "Failed to close file",
    "Unknown command line argument",
    "Unknown processor instruction",
    "Unknown register name",
    "Unknown label",
    "Failed to allocate memory"
};


CommandInfo commands[] = {
    {CMD_HLT,   "HLT",   0, asmCmdNoArgs},
    {CMD_PUSH,  "PUSH",  0, asmCmdPush},
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
    {CMD_PUSHR, "PUSHR", 0, asmCmdRegs},
    {CMD_POPR,  "POPR",  0, asmCmdRegs},
    {CMD_PUSHM, "PUSHM", 0, asmCmdMemoryRegs},
    {CMD_POPM,  "POPM",  0, asmCmdMemoryRegs}
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


ErrorCode initializeBuffer(AssemblyData* asmdata)
{
    assert(asmdata != NULL);
    assert(asmdata->args.input_file != NULL);

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
    assert(asmdata != NULL);
    assert(asmdata->code.data != NULL);

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
    assert(asmdata != NULL);
    assert(asmdata->label_table.labels != NULL);

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

    TRY_OR_RETURN(parseArguments(asmdata, argc, argv));
    TRY_OR_RETURN(initializeBuffer(asmdata));
    TRY_OR_RETURN(initializeByteCode(asmdata, START_CAPACITY));
    TRY_OR_RETURN(initializeLabelTables(asmdata, START_CAPACITY));

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
