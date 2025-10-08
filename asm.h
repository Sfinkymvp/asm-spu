#ifndef _ASM_H_
#define _ASM_H_


#include "asm_data.h"


ErrorCode assembler(ByteCode* code, char* buffer);


ErrorCode assembleInstruction(ByteCode* code, char* substring, char* instruction, size_t* index);


ErrorCode asmPushValue(ByteCode* code, const char* substring, size_t* index);


ErrorCode getRegister(Register* reg, const char* substring);


ErrorCode asmPushRegister(ByteCode* code, const char* substring, size_t* index);


ErrorCode asmPopRegister(ByteCode* code, const char* substring, size_t* index);


ErrorCode asmJump(ByteCode* code, const char* substring, const char* instruction, size_t* index);


ErrorCode initializeByteCode(ByteCode* code, size_t start_capacity);


ErrorCode expandByteCode(ByteCode* code);


void destroyData(AssemblyData* data);


#endif // _ASM_H_
