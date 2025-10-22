#ifndef _ASM_HASH_H_
#define _ASM_HASH_H_


#include "asm_data.h"


size_t hashDjb2(const unsigned char* line);


void initializeAndSortHashes();


void initializeAndSortCmdHashes(CommandInfo* commands, size_t count);


void initializeAndSortRegisterHashes(RegisterInfo* registers, size_t count);


#endif // _ASM_HASH_H_
