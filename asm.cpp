#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "asm.h"
#include "asm_data.h"
#include "asm_labels.h"


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
    "Failed to allocate memory"};


ErrorCode assembler(AssemblyData* asmdata)
{
    assert(asmdata != NULL);
    assert(asmdata->buffer != NULL);
    assert(asmdata->label_table.labels != NULL);
    assert(asmdata->refs_table.labels != NULL);

    ErrorCode err = ERR_OK;
    err = initializeByteCode(&asmdata->code, START_CAPACITY);
    if (err != ERR_OK)
        return err;

    size_t index = 0;
    char* buffer_copy = strdup(asmdata->buffer);
    if (buffer_copy == NULL)
        return ERR_OUT_OF_MEMORY;
    err = processInstructions(asmdata, buffer_copy, &index);
    free(buffer_copy);
    if (err != ERR_OK)
        return err;

    printf("before processLabelReferences\n");
    printf("label count: %zu, label capacity: %zu\n", asmdata->label_table.count, asmdata->label_table.capacity);
    printf("fixup count: %zu, fixup capacity: %zu\n", asmdata->refs_table.count, asmdata->refs_table.capacity);
    err = processLabelReferences(asmdata);
    if (err != ERR_OK)
        return err;

    asmdata->code.instruction_count = index;
    return ERR_OK;
}


ErrorCode processInstructions(AssemblyData* asmdata, char* buffer, size_t* index)
{
    assert(asmdata != NULL);
    assert(asmdata->buffer != NULL);
    assert(asmdata->code.data != NULL);
    assert(asmdata->label_table.labels != NULL);
    assert(asmdata->refs_table.labels != NULL);
    assert(buffer != NULL);
    assert(index != NULL);

    char instruction[MAX_INSTRUCTION_LEN + 1] = "";
    char* token = strtok(buffer, "\n");
    ErrorCode err = ERR_OK;
    while (token != NULL && err == ERR_OK) {
        if (*index >= asmdata->code.capacity - 1)
            if ((err = expandByteCode(&asmdata->code)) != ERR_OK)
                break;

        err = assembleInstruction(asmdata, token, instruction, index);
        token = strtok(NULL, "\n");
    }

    if (err != ERR_OK) {
        printf("index error: %zu\n", *index);
        return err;
    }

    return ERR_OK;

}


ErrorCode assembleInstruction(AssemblyData* asmdata, char* substring, char* instruction, size_t* index)
{
    assert(asmdata != NULL);
    assert(asmdata->buffer != NULL);
    assert(asmdata->code.data != NULL);
    assert(asmdata->label_table.labels != NULL);
    assert(asmdata->refs_table.labels != NULL);
    assert(substring != NULL);
    assert(instruction != NULL);
    assert(index != NULL);

//    printf("in assembleInstruction: ic - %zu, capacity - %zu\n", asmdata->code.instruction_count, asmdata->code.capacity);
//    printf("in assembleInstruction: index - %zu, data - %d\n", *index, asmdata->code.data[*index]);
    if (sscanf(substring, "%5s", instruction) != 1) {
        printf("YOY\n");
        return ERR_INVALID_INSTRUCTION;
    }

    int* data = asmdata->code.data; 
    ErrorCode err = ERR_OK;
    if (instruction[0] == ':')
        err = asmLabel(&asmdata->label_table, substring, index);
    else if (strcmp(instruction, "HLT") == 0)
        data[(*index)++] = (int)CMD_HLT;
    else if (strcmp(instruction, "PUSH") == 0)
        err = asmPushValue(&asmdata->code, substring, index);
    else if (strcmp(instruction, "ADD") == 0)
        data[(*index)++] = (int)CMD_ADD;
    else if (strcmp(instruction, "SUB") == 0)
        data[(*index)++] = (int)CMD_SUB;
    else if (strcmp(instruction, "DIV") == 0)
        data[(*index)++] = (int)CMD_DIV;
    else if (strcmp(instruction, "MUL") == 0)
        data[(*index)++] = (int)CMD_MUL;
    else if (strcmp(instruction, "SQRT") == 0)
        data[(*index)++] = (int)CMD_SQRT;
    else if (strcmp(instruction, "PUSHR") == 0)
        err = asmPushRegister(&asmdata->code, substring, index);
    else if (strcmp(instruction, "POPR") == 0)
        err = asmPopRegister(&asmdata->code, substring, index);
    else if (strcmp(instruction, "IN") == 0)
        asmdata->code.data[(*index)++] = (int)CMD_IN;
    else if (strcmp(instruction, "OUT") == 0)
        asmdata->code.data[(*index)++] = (int)CMD_OUT;
    else if (strcmp(instruction, "RET") == 0)
        asmdata->code.data[(*index)++] = (int)CMD_RET;
    else if (asmLabels(asmdata, substring, instruction, index) != ERR_OK)
        return ERR_INVALID_INSTRUCTION;

    if (err != ERR_OK)
        return err;
    return ERR_OK;
}


ErrorCode asmPushValue(ByteCode* code, const char* substring, size_t* index)
{
    assert(code != NULL);
    assert(index != NULL);

    int value = 0;
    if (sscanf(substring, "%*s %d", &value) != 1)
        return ERR_INVALID_INSTRUCTION;

//    printf("add value to data: %d, index: %zu\n", CMD_PUSH, *index);
    code->data[*index] = (int)CMD_PUSH;
//    printf("add value to data: %d, index: %zu\n", value, *index + 1);
    code->data[++(*index)] = value;
    (*index)++;

    return ERR_OK;
}


ErrorCode getRegister(Register* reg, const char* substring)
{
    assert(reg != NULL);
    assert(substring != NULL);

    char reg_name[MAX_REGISTER_LEN + 1] = "";
    if (sscanf(substring, "%*s %s", reg_name) != 1)
        return ERR_INVALID_REGISTER;

    if (strcmp(reg_name, "RESERVED") == 0) *reg = RESERVED;
    else if (strcmp(reg_name, "RAX") == 0) *reg = RAX;
    else if (strcmp(reg_name, "RBX") == 0) *reg = RBX;
    else if (strcmp(reg_name, "RCX") == 0) *reg = RCX;
    else if (strcmp(reg_name, "RDX") == 0) *reg = RDX;
    else if (strcmp(reg_name, "REX") == 0) *reg = REX;
    else if (strcmp(reg_name, "RFX") == 0) *reg = RFX;
    else if (strcmp(reg_name, "RGX") == 0) *reg = RGX;
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

//    printf("add value to data: %d, index: %zu\n", CMD_PUSHR, *index + 1);
    code->data[*index] = (int)CMD_PUSHR;
//    printf("add value to data: %d, index: %zu\n", reg, *index);
    code->data[++(*index)] = (int)reg;
    (*index)++;

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

//    printf("add value to data: %d, index: %zu\n", CMD_POPR, *index);
    code->data[*index] = (int)CMD_POPR;
//    printf("add value to data: %d, index: %zu\n", reg, *index + 1);
    code->data[++(*index)] = (int)reg;
    (*index)++;

    return ERR_OK;
}


ErrorCode asmLabel(LabelTable* label_table, const char* substring, size_t* index)
{
    assert(label_table != NULL);
    assert(label_table->labels != NULL);
    assert(substring != NULL);
    assert(index != NULL);

    ErrorCode err = ERR_OK;
    if (label_table->count == label_table->capacity)
        err = labelTableExpand(label_table);
    if (err != ERR_OK)
        return err;

    char label_name[LABEL_NAME_MAX] = "";
    if (sscanf(substring, ":%" LABEL_LEN_MAX_STR "s", label_name) != 1) {
        printf("Label string: %s\n", label_name);
        return ERR_INVALID_LABEL;
    }
//    printf("index: int %d, size_t %zu\n", *index, *index);
    addLabel(&label_table->labels[label_table->count++], label_name, (int)*index);
    return ERR_OK;
}


ErrorCode processLabelReferences(AssemblyData* asmdata)
{
    assert(asmdata != NULL);
    assert(asmdata->buffer != NULL);
    assert(asmdata->code.data != NULL);
    assert(asmdata->label_table.labels != NULL);
    assert(asmdata->refs_table.labels != NULL);

    for (size_t index = 0; index < asmdata->refs_table.count; index++) {
        int address = getLabelAddress(&asmdata->label_table,
                                       asmdata->refs_table.labels[index].name);
        if (address == WAIT_LABEL) {
            printf("I FIND ERROR\n");
            printf("String: %s\n", asmdata->refs_table.labels[index].name);
            for (size_t ind = 0; ind < asmdata->refs_table.count; ind++)
                printf("* %s\n", asmdata->refs_table.labels[ind].name);
            return ERR_INVALID_LABEL;
        }

        int bytecode_address = (int)asmdata->refs_table.labels[index].address;
//        printf("bytecode_address: %d, address = %d\n", bytecode_address, address);
        asmdata->code.data[bytecode_address] = address;
    }

    return ERR_OK;
}


ErrorCode asmLabels(AssemblyData* asmdata, const char* substring, const char* instruction, size_t* index)
{   
    assert(asmdata != NULL);
    assert(asmdata->code.data != NULL);
    assert(substring != NULL);
    assert(instruction != NULL);
    assert(index != NULL);

    ErrorCode err = parseLabelInstruction(&asmdata->code.data[*index], instruction);
    if (err != ERR_OK) {
        printf("Error in ParseLabelInstruction(asmLabels)\n");
        return err;
    }
    
    char label_name[LABEL_NAME_MAX] = "";
    if (sscanf(substring, "%*s :%s", label_name) != 1) {
        printf("error string: %s\n", substring);
        return ERR_INVALID_LABEL;
    }
   
    int address = getLabelAddress(&asmdata->label_table, label_name);
    if (address != WAIT_LABEL) {
//       printf("address: %d\n", address);
        asmdata->code.data[++(*index)] = address;
    } else {
//        printf("label name: %s, index: %zu\n", label_name, *index + 1);
//        printf("index int: %d\n", (int)*index + 1);
        addLabel(&asmdata->refs_table.labels[asmdata->refs_table.count++], label_name, (int)++(*index));
//        printf("refs_table.count = %zu, label_table.count = %zu\n",
//                asmdata->refs_table.count, asmdata->label_table.count
    }
    (*index)++;

    return ERR_OK;
}


ErrorCode parseLabelInstruction(int* element, const char* instruction)
{
    if      (strcmp(instruction, "CALL") == 0) *element = (int)CMD_CALL;
    else if (strcmp(instruction, "JMP") == 0)  *element = (int)CMD_JMP;
    else if (strcmp(instruction, "JB")  == 0)  *element = (int)CMD_JB;
    else if (strcmp(instruction, "JBE") == 0)  *element = (int)CMD_JBE;
    else if (strcmp(instruction, "JA")  == 0)  *element = (int)CMD_JA;
    else if (strcmp(instruction, "JAE") == 0)  *element = (int)CMD_JAE;
    else if (strcmp(instruction, "JE")  == 0)  *element = (int)CMD_JE;
    else if (strcmp(instruction, "JNE") == 0)  *element = (int)CMD_JNE;
    else
        return ERR_INVALID_INSTRUCTION;

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


void assemblerDtor(AssemblyData* asmdata)
{
    assert(asmdata != NULL);
   
    free(asmdata->buffer);
    free(asmdata->code.data);
    free(asmdata->refs_table.labels);
    free(asmdata->label_table.labels);
}
