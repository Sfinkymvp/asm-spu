#ifndef _ASM_UTILS_H_
#define _ASM_UTILS_H_


#include "asm_data.h"


ErrorCode getWord(char** dest, char* line, size_t word_index);


ErrorCode getRegister(RegisterInfo* reg_table, Register* reg, const char* line);


ErrorCode getNumber(long* value, char* line);


#endif // _ASM_UTILS_H_
