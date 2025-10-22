#ifndef _ASM_HASH_H_
#define _ASM_HASH_H_


#include "asm_data.h"


size_t hashDjb2(const unsigned char* line);


ssize_t binarySearch(const void* key, const void* base, size_t count,
                 size_t size, int (*compar)(const void*, const void*));


int compareCmdHashesBin(const void* ptr1, const void* ptr2);


int compareCmdHashesSort(const void* ptr1, const void* ptr2);


int compareRegisterHashesBin(const void* ptr1, const void* ptr2);


int compareRegisterHashesSort(const void* ptr1, const void* ptr2);


void initializeAndSortHashes();


void initializeAndSortCmdHashes(CommandInfo* commands, size_t count);


void initializeAndSortRegisterHashes(RegisterInfo* registers, size_t count);


#endif // _ASM_HASH_H_
