#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>


#include "asm_utils.h"
#include "asm_data.h"
#include "asm_hash.h"


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


ErrorCode getRegister(RegisterInfo* reg_table, Register* reg, const char* reg_name)
{
    assert(reg_table != NULL);
    assert(reg != NULL);
    assert(reg_name != NULL);

    size_t reg_hash = hashDjb2((const unsigned char*)reg_name);
    for (size_t index = 0; index < REGISTER_COUNT; index++) {
        if (reg_table[index].hash == reg_hash &&
            strcmp(reg_table[index].name, reg_name) == 0) {
            *reg = reg_table[index].code;
            return ERR_OK;
        }
    }

    return ERR_INVALID_REGISTER;
}


ErrorCode getNumber(long* value, char* line)
{
    assert(value != NULL);
    assert(line != NULL);

    char* end_line = NULL;
    *value = strtol(line, &end_line, 10);
    if (*end_line != '\0' || *value == LONG_MIN || *value == LONG_MAX)
        return ERR_INVALID_OPERAND;

    return ERR_OK;
}
