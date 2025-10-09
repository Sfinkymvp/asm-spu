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
    printf("after ctor\n"); parseArguments(&cpu.args, (size_t)argc, argv);

    assert(cpu.args.input_file != NULL);
    printf("after parse\n");

    loadByteCode(&cpu.code, cpu.args.input_file);
    printf("after load code\n");

    printf("Processor exit code: %d\n", (int)executeProcessor(&cpu));
    printf("after execute\n");    

    stackDtor(&cpu.stack);
    free(cpu.code.buffer);

    printf("program completion\n");
    return 0;
}
