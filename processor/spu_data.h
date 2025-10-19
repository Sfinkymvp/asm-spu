#ifndef _SPU_DATA_H_
#define _SPU_DATA_H_


#include <stdio.h>

#include "../../secure_stack/stack.h"
#include "../assembler/instructions.h"


const size_t START_STACK_CAPACITY = 8;
const size_t CMD_TABLE_SIZE = (size_t)CMD_POPM + 1;
const size_t RAM_SIZE = 100;


extern const char* error_messages[];


#define STACK_ERR(function_call)                     \
    do {                                             \
        if ((function_call) != SUCCESS)              \
            return (ERR_STACK);                      \
    } while (0)


typedef enum {
    ERR_OK = 0,
    ERR_FILE_OPEN,
    ERR_FILE_READ,
    ERR_FILE_WRITE,
    ERR_FILE_CLOSE,
    ERR_STACK,
    ERR_INVALID_CMD_ARGUMENT,
    ERR_INVALID_INSTRUCTION,
    ERR_INVALID_MEMORY_ACCESS,
    ERR_INVALID_OPERATION,
    ERR_INVALID_REGISTER,
    ERR_INVALID_INPUT,
    ERR_INVALID_OUTPUT,
    ERR_OUT_OF_MEMORY,
    ERR_EXIT
} ErrorCode;


typedef struct {
    const char* input_file;
} Arguments;


typedef struct {
    int* data;
    size_t capacity;
} ByteCode;


typedef struct {
    Arguments args;
    Stack_t stack; 
    Stack_t call_stack;
    ByteCode bytecode;
    int registers[REGISTER_COUNT];
    int ram[RAM_SIZE];
    int ip;
} Processor;


typedef struct CommandInfo CommandInfo;
struct CommandInfo {
    Instruction code;
    ErrorCode (*handler)(Processor*);
};


extern CommandInfo commands[];


void initializeCommands();

#endif // _SPU_DATA_H_
