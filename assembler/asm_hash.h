#ifndef _ASM_HASH_H_
#define _ASM_HASH_H_


#include "asm_data.h"


size_t hash_djb2(const unsigned char* line);


void initializeHashes();


void initializeCmdHashes(CommandInfo* commands, size_t count);


void initializeRegisterHashes(RegisterInfo* registers, size_t count);


#endif // _ASM_HASH_H_
