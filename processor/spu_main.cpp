#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>


#include "spu_data.h"
#include "spu.h"
#include "spu_input.h"
#include "../../secure_stack/stack.h"


int main(int argc, const char** argv)
{
    initializeCommands();

    Processor spu = {};

    stackCtor(&spu.stack, START_STACK_CAPACITY);
    stackCtor(&spu.call_stack, START_STACK_CAPACITY);
    printf("after ctor\n");

    parseArguments(&spu, argc, argv);
    assert(spu.args.input_file != NULL);
    printf("after parse\n");

    loadByteCode(&spu, spu.args.input_file);
    printf("after load code\n");

    printf("Processor exit code: %d\n", (int)runProcessor(&spu));
    printf("ip: %d\n", spu.ip);
    printf("after execute\n");    

    stackDtor(&spu.stack);
    stackDtor(&spu.call_stack);
    free(spu.bytecode.data);

    printf("program completion\n");
    return 0;
}
