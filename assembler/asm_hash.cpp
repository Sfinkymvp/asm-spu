#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


#include "asm_hash.h"
#include "asm_data.h"


size_t hashDjb2(const unsigned char* line)
{
    assert(line != NULL);

    size_t hash = 5381;
    for (size_t index = 0; line[index] != '\0'; index++)
        hash = ((hash << 5) + hash) + line[index];

    return hash;
}


ssize_t binarySearch(const void* key, const void* base, size_t count,
                 size_t size, int (*compar)(const void*, const void*))
{
    assert(key != NULL);
    assert(base != NULL);
    assert(compar != NULL);

    ssize_t left = 0;
    ssize_t right = (ssize_t)count - 1;

    while (left <= right) {
        ssize_t mid = left + (right - left) / 2;
        int cmp_result = compar(key, (const char*)base + mid * (ssize_t)size);

        if (cmp_result == 0)
            return mid;
        else if (cmp_result < 0)
            right = mid - 1;
        else
            left = mid + 1;
    }

    return -1;
}


int compareCmdHashesBin(const void* ptr1, const void* ptr2)
{
    assert(ptr1 != NULL);
    assert(ptr2 != NULL);

    size_t hash_1 = *(const size_t*)ptr1;
    size_t hash_2 = ((const CommandInfo*)ptr2)->hash;

    return (hash_1 > hash_2) - (hash_1 < hash_2);
}


int compareCmdHashesSort(const void* ptr1, const void* ptr2)
{
    assert(ptr1 != NULL);
    assert(ptr2 != NULL);

    size_t hash_1 = ((const CommandInfo*)ptr1)->hash;
    size_t hash_2 = ((const CommandInfo*)ptr2)->hash;

    return (hash_1 > hash_2) - (hash_1 < hash_2);
}


int compareRegisterHashesBin(const void* ptr1, const void* ptr2)
{
    assert(ptr1 != NULL);
    assert(ptr2 != NULL);

    size_t hash_1 = *(const size_t*)ptr1;
    size_t hash_2 = ((const RegisterInfo*)ptr2)->hash;

    return (hash_1 > hash_2) - (hash_1 < hash_2);
}


int compareRegisterHashesSort(const void* ptr1, const void* ptr2)
{
    assert(ptr1 != NULL);
    assert(ptr2 != NULL);

    size_t hash_1 = ((const RegisterInfo*)ptr1)->hash;
    size_t hash_2 = ((const RegisterInfo*)ptr2)->hash;

    return (hash_1 > hash_2) - (hash_1 < hash_2);
}


void initializeAndSortHashes()
{
    initializeAndSortCmdHashes(commands, COMMAND_COUNT);
    initializeAndSortRegisterHashes(regs, REGISTER_COUNT);
    initializeAndSortRegisterHashes(memory_regs, REGISTER_COUNT);
}


void initializeAndSortCmdHashes(CommandInfo* command_table, size_t count)
{
    assert(command_table != NULL);

    for (size_t index = 0; index < count; index++)
        command_table[index].hash = hashDjb2((const unsigned char*)command_table[index].name);

    qsort(command_table, count, sizeof(CommandInfo), compareCmdHashesSort);
}


void initializeAndSortRegisterHashes(RegisterInfo* reg_table, size_t count)
{
    assert(reg_table != NULL);

    for (size_t index = 0; index < count; index++)
        reg_table[index].hash = hashDjb2((const unsigned char*)reg_table[index].name);

    qsort(reg_table, count, sizeof(RegisterInfo), compareRegisterHashesSort);
}
