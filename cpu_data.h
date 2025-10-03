#ifndef _CPU_DATA_H_
#define _CPU_DATA_H_


typedef enum {
    HLT = 0,
    PUSH,
    ADD,
    SUB,
    DIV,
    MUL,
    SQRT,
    OUT
} Instruction;


typedef struct {
    int* data;
    size_t size;
    size_t capacity;
} Program;


#endif // _CPU_DATA_H_
