#ifndef _TRANSLATOR_DATA_H_
#define _TRANSLATOR_DATA_H_


typedef enum {
    PUSH = 0,
    ADD,
    SUB,
    DIV,
    MUL,
    SQRT,
    OUT,
    HLT
} Instruction;


typedef struct {
    int* data;
    size_t size;
    size_t capacity;
} Program;

#endif // _TRANSLATOR_DATA_H_
