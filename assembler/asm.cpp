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
    assert(asmdata != NULL);
    assert(asmdata->buffer != NULL);
    assert(asmdata->label_table.labels != NULL);
    assert(asmdata->fixups.labels != NULL);

    TRY_OR_RETURN(processInstructions(asmdata));
    TRY_OR_RETURN(processLabelFixups(asmdata));

    return ERR_OK;
}


ErrorCode processInstructions(AssemblyData* asmdata)
{
    assert(asmdata != NULL);
    assert(asmdata->buffer != NULL);
    assert(asmdata->code.data != NULL);
    assert(asmdata->label_table.labels != NULL);
    assert(asmdata->fixups.labels != NULL);

    char* token = strtok(asmdata->buffer, "\n");
    char* last_token = NULL;
    ErrorCode err = ERR_OK;
    while (token != NULL && err == ERR_OK) {
        if (asmdata->ip >= (int)asmdata->code.capacity - 1)
            if ((err = expandByteCode(asmdata)) != ERR_OK)
                break;

        last_token = token;
        token = strtok(NULL, "\n");
        err = assembleInstruction(asmdata, last_token);
    }

    return err;
}


ErrorCode assembleInstruction(AssemblyData* asmdata, char* line)
{
    assert(asmdata != NULL);
    assert(asmdata->buffer != NULL);
    assert(asmdata->code.data != NULL);
    assert(asmdata->label_table.labels != NULL);
    assert(asmdata->fixups.labels != NULL);

    char instruction[NAME_MAX_LEN] = "";
    if (sscanf(line, "%s", instruction) != 1)
        return ERR_INVALID_INSTRUCTION;

    if (instruction[0] == ':') {
        char* word = NULL;
        TRY_OR_RETURN(getWord(&word, line, 1));
        return defineLabel(asmdata, word + 1);
    }

    ErrorCode err = ERR_OK;
    for (size_t index = 0; index < COMMAND_COUNT && err == ERR_OK; index++) {
        if (commands[index].hash == hash_djb2((const unsigned char*)instruction) &&
            strcmp(commands[index].name, instruction) == 0) {
            err = commands[index].handler(asmdata, &commands[index], line);
            break;
        }
    }

    return err;
}


ErrorCode getWord(char** dest, char* line, size_t word_index)
{
    assert(line != NULL);
    assert(dest != NULL);

    while (*line == ' ' || *line == '\t')
        line++;

    for (size_t index = 1; index < word_index; index++) {
        line = strpbrk(line, " \t");
        if (line == NULL) {
            printf("null line\n");
            return ERR_INVALID_OPERAND;
        }

        while (*line == ' ' || *line == '\t')
            line++;
        if (*line == '\0') {
            printf("\\0 is line\n");
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
    TRY_OR_RETURN(getWord(&reg_name, line, 2));

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
