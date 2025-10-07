#ifndef _CPU_DATA_H_
#define _CPU_DATA_H_


#include <stddef.h>

#include "../secure_stack/stack.h"
#include "instructions.h"


const size_t START_STACK_CAPACITY = 8;


extern const char* error_messages[];


typedef enum {
    ERR_OK = 0,
    ERR_FILE_OPEN,
    ERR_FILE_READ,
    ERR_FILE_WRITE,
    ERR_FILE_CLOSE,
    ERR_INVALID_CMD_ARGUMENT,
    ERR_INVALID_INSTRUCTION,
    ERR_INVALID_INPUT,
    ERR_INVALID_OUTPUT,
    ERR_OUT_OF_MEMORY,
    ERR_STACK
} ErrorCode;


typedef enum {
    RESERVED = 0,
    RAX,
    RBX,
    RCX,
    RDX,
    REX,
    RFX,
    RGX
} Register;


const size_t REGISTER_COUNT = RGX + 1;


typedef struct {
    const char* input_file;
} Arguments;


typedef struct {
    int* buffer;
    size_t instruction_count;
    size_t capacity;
} ByteCode;


typedef struct {
    Arguments args;
    Stack_t stack; 
    ByteCode code;
    size_t ip;
    int registers[REGISTER_COUNT];
} Processor;


#endif // _CPU_DATA_H_
