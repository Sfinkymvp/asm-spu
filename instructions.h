#ifndef _INSTRUCTIONS_H_
#define _INSTRUCTIONS_H_


typedef enum {
    CMD_HLT   =  0,
    CMD_PUSH  =  1,
    CMD_ADD   =  2,
    CMD_SUB   =  3,
    CMD_DIV   =  4,
    CMD_MUL   =  5,
    CMD_SQRT  =  6,
    CMD_IN    =  7,
    CMD_OUT   =  8,
    CMD_JMP   =  11,
    CMD_JB    =  12,
    CMD_JBE   =  13,
    CMD_JA    =  14,
    CMD_JAE   =  15,
    CMD_JE    =  16,
    CMD_JNE   =  17,
    CMD_CALL  =  18,
    CMD_RET   =  19,
    CMD_PUSHR =  33,
    CMD_POPR  =  34
} Instruction;


const int MAX_INSTRUCTION_LEN = 5;


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


const int MAX_REGISTER_LEN = 8;


#endif // _INSTRUCTIONS_H_
