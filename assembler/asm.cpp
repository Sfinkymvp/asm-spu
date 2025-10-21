#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


#include "asm.h"
#include "asm_data.h"
#include "asm_labels.h"
#include "asm_hash.h"


ErrorCode assembler(AssemblyData* asmdata)
{
    ASSERT_ASM(asmdata);

    CHECK_OK(processInstructions(asmdata));
    CHECK_OK(processLabelFixups(asmdata));

    return ERR_OK;
}


ErrorCode processInstructions(AssemblyData* asmdata)
{
    ASSERT_ASM(asmdata);

    char* token = strtok(asmdata->buffer, "\n");
    char* last_token = NULL;
    ErrorCode err = ERR_OK;
    while (token != NULL && err == ERR_OK) {
        if (asmdata->ip >= (int)asmdata->code.capacity - 1)
            if ((err = expandByteCode(asmdata)) != ERR_OK)
                break;

        last_token = token;
        err = assembleInstruction(asmdata, last_token);
        token = strtok(NULL, "\n");
    }

    return err;
}


ErrorCode assembleInstruction(AssemblyData* asmdata, char* line)
{
    ASSERT_ASM(asmdata);

    char instruction[NAME_MAX_LEN] = "";
    if (sscanf(line, "%s", instruction) != 1)
        return ERR_INVALID_INSTRUCTION;

    if (instruction[0] == ':') {
        char* word = NULL;
        CHECK_OK(getWord(&word, line, 1));
        return defineLabel(asmdata, word + 1);
    }

    size_t instruction_hash = hash_djb2((const unsigned char*)instruction);
    ErrorCode err = ERR_OK;
    for (size_t index = 0; index < COMMAND_COUNT && err == ERR_OK; index++) {
        if (commands[index].hash == instruction_hash &&
            strcmp(commands[index].name, instruction) == 0) {
            err = commands[index].handler(asmdata, &commands[index], line);
            break;
        }
    }

    return err;
}


ErrorCode getWord(char** dest, char* line, size_t word_index)
{
    assert(dest != NULL);
    assert(line != NULL);

    while (*line == ' ' || *line == '\t')
        line++;

    for (size_t index = 1; index < word_index; index++) {
        line = strpbrk(line, " \t");
        if (line == NULL) {
            return ERR_INVALID_OPERAND;
        }

        while (*line == ' ' || *line == '\t')
            line++;
        if (*line == '\0') {
            return ERR_INVALID_OPERAND;
        }
    }

    *dest = line;
    line = strpbrk(line, " \t");
    if (line != NULL)
        *line = '\0';

    return ERR_OK;
}


ErrorCode getRegister(RegisterInfo* reg_table, Register* reg, char* line)
{
    assert(reg_table != NULL);
    assert(reg != NULL);
    assert(line != NULL);

    char* reg_name = NULL;
    CHECK_OK(getWord(&reg_name, line, 2));

    assert(reg_name != NULL);

    size_t reg_hash = hash_djb2((const unsigned char*)reg_name);
    for (size_t index = 0; index < REGISTER_COUNT; index++) {
        if (reg_table[index].hash == reg_hash &&
            strcmp(reg_table[index].name, reg_name) == 0) {
            *reg = reg_table[index].code;
            return ERR_OK;
        }
    }

    return ERR_INVALID_REGISTER;
}
