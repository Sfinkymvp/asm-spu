#ifndef _ASM_H_
#define _ASM_H_


#include "asm_data.h"


ErrorCode assembler(ByteCode* code, char* buffer);


ErrorCode assembleInstruction(ByteCode* code, char* substring, char* instruction, size_t* index);


ErrorCode initializeByteCode(ByteCode* code, size_t start_capacity);


ErrorCode expandByteCode(ByteCode* code);


void destroyData(AssemblyData* data);


#endif // _ASM_H_
