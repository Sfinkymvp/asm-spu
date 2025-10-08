#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>


#include "cpu_data.h"
#include "cpu.h"
#include "cpu_input.h"
#include "../secure_stack/stack.h"


int main(int argc, const char** argv)
{
    Processor cpu = {};

    stackCtor(&cpu.stack, START_STACK_CAPACITY);

    parseArguments(&cpu.args, (size_t)argc, argv);
    assert(cpu.args.input_file != NULL);

    loadByteCode(&cpu.code, cpu.args.input_file);

    printf("%d\n", (int)executeProcessor(&cpu));
    
    stackDtor(&cpu.stack);
    free(cpu.code.buffer);

    return 0;
}
