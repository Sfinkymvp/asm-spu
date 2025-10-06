#ifndef _ASM_OUTPUT_H_
#define _ASM_OUTPUT_H_


#include "asm_data.h"


ErrorCode writeByteCodeToFile(const ByteCode* code, const char* output_filename);


void printError(ErrorCode error_code);


#endif // _ASM_OUTPUT_H_
