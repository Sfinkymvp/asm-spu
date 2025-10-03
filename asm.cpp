#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "asm.h"
#include "asm_data.h"


bool assembler(char* buffer)
{
    FILE* out = fopen("output", "w");
    if (out == NULL)
        return false;

    char instruction[5] = "";
    char* token = strtok(buffer, "\n");
    int value;

    while (token != NULL) {
        if (sscanf(token, "%4s", instruction) <= 0)
            return false;

        if (strcmp(instruction, "PUSH") == 0) {
            if (sscanf(token, "%*s %d", &value) == 1)
                fprintf(out, "%d %d\n", (int)PUSH, value);
            else
                return false;
        } else if (strcmp(instruction, "ADD") == 0) {
            fprintf(out, "%d\n", (int)ADD);
        } else if (strcmp(instruction, "SUB") == 0) {
            fprintf(out, "%d\n", (int)SUB);
        } else if (strcmp(instruction, "DIV") == 0) {
            fprintf(out, "%d\n", (int)DIV);
        } else if (strcmp(instruction, "MUL") == 0) {
            fprintf(out, "%d\n", (int)MUL);
        } else if (strcmp(instruction, "SQRT") == 0) {
            fprintf(out, "%d\n", (int)SQRT);
        } else if (strcmp(instruction, "OUT") == 0) {
            fprintf(out, "%d\n", (int)OUT);
        } else if (strcmp(instruction, "HLT") == 0) {
            fprintf(out, "%d", (int)HLT);
        } else
            return false;

        token = strtok(NULL, "\n");
    }

    fclose(out);

    return true;
}
