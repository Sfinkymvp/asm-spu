#ifndef _SPU_DATA_H_
#define _SPU_DATA_H_


#include <stdio.h>


#include "../../secure_stack/stack.h"
#include "../assembler/instructions.h"


const size_t START_STACK_CAPACITY = 8;
const size_t CMD_TABLE_SIZE = (size_t)CMD_DRAW + 1;


const size_t DRAW_WIDTH = 80;
const size_t DRAW_HEIGHT = 40;
const size_t RAM_SIZE = DRAW_WIDTH * DRAW_HEIGHT;


typedef enum {
    ERR_OK = 0,
    ERR_FILE_OPEN,
    ERR_FILE_READ,
    ERR_FILE_CLOSE,
    ERR_STACK,
    ERR_INVALID_CMD_ARGUMENT,
    ERR_INVALID_INSTRUCTION,
    ERR_INVALID_MEMORY_ACCESS,
    ERR_INVALID_ENTER,
    ERR_INVALID_REGISTER,
    ERR_OUT_OF_MEMORY,
    ERR_EXIT
} ErrorCode;


const size_t ERROR_TABLE_SIZE = (size_t)ERR_EXIT + 1;


typedef struct {
    int count;
    const char** values;
    int pos;
} CmdArgs;


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


typedef ErrorCode (*CommandHandler)(Processor*);


extern CommandHandler commands[];


ErrorCode spuCtor(Processor* spu);


ErrorCode spuDtor(Processor* spu);


void initializeCommands();


#endif // _SPU_DATA_H_
