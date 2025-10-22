#ifndef _ASM_H_
#define _ASM_H_


#include "asm_data.h"


ErrorCode assembler(AssemblyData* asmdata);


ErrorCode processInstructions(AssemblyData* asmdata);


ErrorCode assembleInstruction(AssemblyData* asmdata, char* line);


#endif // _ASM_H_
