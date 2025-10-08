#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "asm.h"
#include "asm_data.h"


const char* error_messages[] = {
    "This message should not be printed",
    "Failed to open file",
    "Failed to read data from file",
    "Failed to write data to file",
    "Failed to close file",
    "Unknown command line argument",
    "Unknown processor instruction",
    "Unknown register name",
    "Failed to allocate memory"};


ErrorCode assembler(ByteCode* code, char* buffer)
{
    assert(code != NULL);
    assert(buffer != NULL);

    initializeByteCode(code, START_CAPACITY);

    char instruction[MAX_INSTRUCTION_LEN + 1] = "";
    char* token = strtok(buffer, "\n");
    size_t index = 0;
    ErrorCode error_code = ERR_OK;

    while (token != NULL && error_code == ERR_OK) {
        if (index >= code->capacity - 1)
            error_code = expandByteCode(code);

        if (error_code != ERR_OK)
            break;
        error_code = assembleInstruction(code, token, instruction, &index);

        token = strtok(NULL, "\n");
    }

    if (error_code != ERR_OK) {
        free(code->data);
        code->data = NULL;
        return error_code;
    }

    code->instruction_count = index;
    return ERR_OK;
}


ErrorCode assembleInstruction(ByteCode* code, char* substring, char* instruction, size_t* index)
{
    assert(code != NULL);
    assert(substring != NULL);
    assert(instruction != NULL);
    assert(index != NULL);

    if (sscanf(substring, "%5s", instruction) != 1) {
        return ERR_INVALID_INSTRUCTION;
    }

    ErrorCode err = ERR_OK;
    if (strcmp(instruction, "HLT") == 0) {
        code->data[*index] = (int)CMD_HLT;
    } else if (strcmp(instruction, "PUSH") == 0) {
        err = asmPushValue(code, substring, index);
    } else if (strcmp(instruction, "ADD") == 0) {
        code->data[*index] = (int)CMD_ADD;
    } else if (strcmp(instruction, "SUB") == 0) {
        code->data[*index] = (int)CMD_SUB;
    } else if (strcmp(instruction, "DIV") == 0) {
        code->data[*index] = (int)CMD_DIV;
    } else if (strcmp(instruction, "MUL") == 0) {
        code->data[*index] = (int)CMD_MUL;
    } else if (strcmp(instruction, "SQRT") == 0) {
        code->data[*index] = (int)CMD_SQRT;
    } else if (strcmp(instruction, "PUSHR") == 0) {
        err = asmPushRegister(code, substring, index);
    } else if (strcmp(instruction, "POPR") == 0) {
        err = asmPopRegister(code, substring, index);
    } else if (strcmp(instruction, "IN") == 0) {
        code->data[*index] = (int)CMD_IN;
    } else if (strcmp(instruction, "OUT") == 0) {
        code->data[*index] = (int)CMD_OUT;
    } else if (asmJump(code, substring, instruction, index) != ERR_OK) {
        return ERR_INVALID_INSTRUCTION;
    }

    if (err != ERR_OK) {
        return err;
    }

    (*index)++;
    return ERR_OK;
}


ErrorCode asmPushValue(ByteCode* code, const char* substring, size_t* index)
{
    assert(code != NULL);
    assert(index != NULL);

    int value = 0;
    if (sscanf(substring, "%*s %d", &value) != 1)
        return ERR_INVALID_INSTRUCTION;

    code->data[*index] = (int)CMD_PUSH;
    code->data[++(*index)] = value;
    return ERR_OK;
}


ErrorCode getRegister(Register* reg, const char* substring)
{
    assert(reg != NULL);
    assert(substring != NULL);

    char reg_name[MAX_REGISTER_LEN + 1] = "";
    if (sscanf(substring, "%*s %s", reg_name) != 1)
        return ERR_INVALID_REGISTER;

    if (strcmp(reg_name, "RESERVED") == 0)
        *reg = RESERVED;
    else if (strcmp(reg_name, "RAX") == 0)
        *reg = RAX;
    else if (strcmp(reg_name, "RBX") == 0)
        *reg = RBX;
    else if (strcmp(reg_name, "RCX") == 0)
        *reg = RCX;
    else if (strcmp(reg_name, "RDX") == 0)
        *reg = RDX;
    else if (strcmp(reg_name, "REX") == 0)
        *reg = REX;
    else if (strcmp(reg_name, "RFX") == 0)
        *reg = RFX;
    else if (strcmp(reg_name, "RGX") == 0)
        *reg = RGX;
    else
        return ERR_INVALID_REGISTER;

    return ERR_OK;
}


ErrorCode asmPushRegister(ByteCode* code, const char* substring, size_t* index)
{
    assert(code != NULL);
    assert(substring != NULL);
    assert(index != NULL);

    Register reg = RESERVED;
    ErrorCode err = getRegister(&reg, substring);
    if (err != ERR_OK)
        return err;

    code->data[*index] = (int)CMD_PUSHR;
    code->data[++(*index)] = (int)reg;
    return ERR_OK;
}


ErrorCode asmPopRegister(ByteCode* code, const char* substring, size_t* index)
{
    assert(code != NULL);
    assert(substring != NULL);
    assert(index != NULL);

    Register reg = RESERVED;
    ErrorCode err = getRegister(&reg, substring);
    if (err != ERR_OK)
        return err;

    code->data[*index] = (int)CMD_POPR;
    code->data[++(*index)] = (int)reg;
    return ERR_OK;
}


ErrorCode asmJump(ByteCode* code, const char* substring, const char* instruction, size_t* index)
{   
    assert(code != NULL);
    assert(substring != NULL);
    assert(instruction != NULL);
    assert(index != NULL);

    if (strcmp(instruction, "JMP") == 0)
        code->data[*index] = (int)CMD_JMP;
    else if (strcmp(instruction, "JB") == 0)
        code->data[*index] = (int)CMD_JB;
    else if (strcmp(instruction, "JBE") == 0)
        code->data[*index] = (int)CMD_JBE;
    else if (strcmp(instruction, "JA") == 0)
        code->data[*index] = (int)CMD_JA;
    else if (strcmp(instruction, "JAE") == 0)
        code->data[*index] = (int)CMD_JAE;
    else if (strcmp(instruction, "JE") == 0)
        code->data[*index] = (int)CMD_JE;
    else if (strcmp(instruction, "JNE") == 0)
        code->data[*index] = (int)CMD_JNE;
    else
        return ERR_INVALID_INSTRUCTION;

    int value = 0;
    if (sscanf(substring, "%*s %d", &value) != 1)
        return ERR_INVALID_INSTRUCTION;
    code->data[++(*index)] = value;

    return ERR_OK;
}


ErrorCode initializeByteCode(ByteCode* code, size_t start_capacity)
{
    assert(code != NULL);

    code->data = (int*)calloc(start_capacity, sizeof(int));
    if (code->data == NULL)
        return ERR_OUT_OF_MEMORY;

    code->capacity = start_capacity;
    return ERR_OK;
}


ErrorCode expandByteCode(ByteCode* code)
{
    assert(code != NULL);

    void* temp = realloc(code->data, code->capacity * 2 * sizeof(int));
    if (temp == NULL)
        return ERR_OUT_OF_MEMORY;

    code->data = (int*)temp;
    code->capacity *= 2;
    return ERR_OK;
}


void destroyData(AssemblyData* data)
{
    assert(data != NULL);
   
    free(data->buffer);
    free(data->code.data);
}
