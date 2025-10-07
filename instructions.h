#ifndef _INSTRUCTIONS_H_
#define _INSTRUCTIONS_H_


typedef enum {
    CMD_HLT = 0,
    CMD_PUSH = 1,
    CMD_ADD = 2,
    CMD_SUB = 3,
    CMD_DIV = 4,
    CMD_MUL = 5,
    CMD_SQRT = 6,
    CMD_PUSHR = 7,
    CMD_POPR = 8,
    CMD_IN = 9,
    CMD_OUT = 10
} Instruction;


const size_t MAX_INSTRUCTION_LEN = 5;

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


const size_t MAX_REGISTER_LEN = 8;


#endif // _INSTRUCTIONS_H_
