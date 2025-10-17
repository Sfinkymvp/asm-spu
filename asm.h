#ifndef _ASM_H_
#define _ASM_H_


#include "asm_data.h"


ErrorCode assembler(AssemblyData* asmdata);


ErrorCode processInstructions(AssemblyData* asmdata, char* buffer, size_t* index);


ErrorCode assembleInstruction(AssemblyData* asmdata, char* substring, char* instruction, size_t* index);


ErrorCode asmPushValue(ByteCode* code, const char* substring, size_t* index);


ErrorCode getRegister(Register* reg, const char* substring);


ErrorCode asmPushRegister(ByteCode* code, const char* substring, size_t* index);


ErrorCode asmPopRegister(ByteCode* code, const char* substring, size_t* index);


ErrorCode asmLabel(LabelTable* label_table, const char* substring, size_t* index);


ErrorCode processLabelReferences(AssemblyData* asmdata);


ErrorCode asmLabels(AssemblyData* asmdata, const char* substring, const char* instruction, size_t* index);


ErrorCode parseLabelInstruction(int* element, const char* instruction);


ErrorCode initializeByteCode(ByteCode* code, size_t start_capacity);


ErrorCode expandByteCode(ByteCode* code);


void assemblerDtor(AssemblyData* asmdata);


#endif // _ASM_H_
