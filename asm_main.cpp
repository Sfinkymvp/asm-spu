#include <stdio.h>

#include "asm.h"
#include "asm_input.h"


int main()
{
    char* buffer = initializeBuffer("test1");

    if (!assembler(buffer))
        return 1;

    return 0;
}

