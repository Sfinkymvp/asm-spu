#include <stdio.h>
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

}


void initializeAndSortRegisterHashes(RegisterInfo* reg_table, size_t count)
{
    assert(reg_table != NULL);

    for (size_t index = 0; index < count; index++)
        reg_table[index].hash = hashDjb2((const unsigned char*)reg_table[index].name);
}
