#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>


#include "spu.h"
#include "spu_data.h"


ErrorCode runProcessor(Processor* spu)
{
    assert(spu != NULL);
    assert(spu->stack.data != NULL);
    assert(spu->call_stack.data != NULL);
    assert(spu->bytecode.data != NULL);

    ErrorCode err = ERR_OK;
    spu->ip = 0;
    while (spu->ip++ < (int)spu->bytecode.capacity) {
        err = commands[spu->bytecode.data[spu->ip - 1]].handler(spu);
        if (err == ERR_EXIT)
            return ERR_OK;
        if (err != ERR_OK)
            break;
    }

    return err;
}


ErrorCode spuCmdHlt(Processor* spu)
{
    assert(spu != NULL);
    assert(spu->stack.data != NULL);
    assert(spu->call_stack.data != NULL);
    assert(spu->bytecode.data != NULL);

    return ERR_EXIT;
}


ErrorCode spuCmdPush(Processor* spu)
{
    assert(spu != NULL);
    assert(spu->stack.data != NULL);
    assert(spu->call_stack.data != NULL);
    assert(spu->bytecode.data != NULL);

    int value = spu->bytecode.data[spu->ip++];
    STACK_ERR(stackPush(&spu->stack, value));

    return ERR_OK;
}


ErrorCode spuCmdArithmetic(Processor* spu)
{
    assert(spu != NULL);
    assert(spu->stack.data != NULL);
    assert(spu->call_stack.data != NULL);
    assert(spu->bytecode.data != NULL);

    int value1 = 0;
    int value2 = 0;
    stackPop(&spu->stack, &value2);
    stackPop(&spu->stack, &value1);

    return spuExecArithmetic(spu, value1, value2, (Instruction)spu->bytecode.data[spu->ip - 1]);
}


ErrorCode spuExecArithmetic(Processor* spu, int value1, int value2, Instruction instruction)
{
    assert(spu != NULL);
    assert(spu->stack.data != NULL);
    assert(spu->call_stack.data != NULL);
    assert(spu->bytecode.data != NULL);

    switch (instruction) {
        case CMD_ADD: STACK_ERR(stackPush(&spu->stack, value1 + value2)); break;
        case CMD_SUB: STACK_ERR(stackPush(&spu->stack, value1 - value2)); break;
        case CMD_MUL: STACK_ERR(stackPush(&spu->stack, value1 * value2)); break;
        case CMD_DIV: STACK_ERR(stackPush(&spu->stack, value1 / value2)); break;
        default:      printf("aboba\n"); return ERR_INVALID_INSTRUCTION;
    }

    return ERR_OK;
}


ErrorCode spuCmdSqrt(Processor* spu)
{
    assert(spu != NULL);
    assert(spu->stack.data != NULL);
    assert(spu->call_stack.data != NULL);
    assert(spu->bytecode.data != NULL);

    int value = 0;
    STACK_ERR(stackPop(&spu->stack, &value));
    STACK_ERR(stackPush(&spu->stack, (int)sqrt(value)));

    return ERR_OK;
}


ErrorCode spuCmdIn(Processor* spu)
{
    assert(spu != NULL);
    assert(spu->stack.data != NULL);
    assert(spu->call_stack.data != NULL);
    assert(spu->bytecode.data != NULL);

    int value = 0; 
    printf("Enter number:\n");
    if (scanf("%d", &value) != 1)
        return ERR_INVALID_OPERATION;

    STACK_ERR(stackPush(&spu->stack, value));
    return ERR_OK;
}


ErrorCode spuCmdOut(Processor* spu)
{
    assert(spu != NULL);
    assert(spu->stack.data != NULL);
    assert(spu->call_stack.data != NULL);
    assert(spu->bytecode.data != NULL);

    int value = 0;
    STACK_ERR(stackPop(&spu->stack, &value));

    printf("OUT: %d\n", value);
    return ERR_OK;
}


ErrorCode spuCmdJump(Processor* spu)
{
    assert(spu != NULL);
    assert(spu->stack.data != NULL);
    assert(spu->call_stack.data != NULL);
    assert(spu->bytecode.data != NULL);

    if (spu->bytecode.data[spu->ip - 1] != CMD_JMP) {
        int value1 = 0;
        int value2 = 0;
        stackPop(&spu->stack, &value2);
        stackPop(&spu->stack, &value1);
        if (!spuJumpCondition(value1, value2, (Instruction)spu->bytecode.data[spu->ip - 1])) {
            spu->ip++;
            return ERR_OK;
        }
    }
  
    spu->ip = spu->bytecode.data[spu->ip]; 
    return ERR_OK;
}


bool spuJumpCondition(int value1, int value2, Instruction instruction)
{
    switch (instruction) {
        case CMD_JB:   return value1 < value2;
        case CMD_JBE:  return value1 <= value2;
        case CMD_JA:   return value1 > value2;
        case CMD_JAE:  return value1 >= value2;
        case CMD_JE:   return value1 == value2;
        case CMD_JNE:  return value1 != value2;
        default:       return false;
    }
}


ErrorCode spuCmdCall(Processor* spu)
{
    assert(spu != NULL);
    assert(spu->stack.data != NULL);
    assert(spu->call_stack.data != NULL);
    assert(spu->bytecode.data != NULL);

    STACK_ERR(stackPush(&spu->call_stack, spu->ip + 1));
    spu->ip = (size_t)spu->bytecode.data[spu->ip];

    return ERR_OK;
}


ErrorCode spuCmdRet(Processor* spu)
{
    assert(spu != NULL);
    assert(spu->stack.data != NULL);
    assert(spu->call_stack.data != NULL);
    assert(spu->bytecode.data != NULL);

    int new_index = 0;
    STACK_ERR(stackPop(&spu->call_stack, &new_index));
    spu->ip = new_index;

    return ERR_OK;
}


ErrorCode spuCmdPushr(Processor* spu)
{
    assert(spu != NULL);
    assert(spu->stack.data != NULL);
    assert(spu->call_stack.data != NULL);
    assert(spu->bytecode.data != NULL);

    int register_id = spu->bytecode.data[spu->ip++];
    if (register_id >= (int)REGISTER_COUNT || register_id < 0)
        return ERR_INVALID_REGISTER;

    STACK_ERR(stackPush(&spu->stack, spu->registers[register_id]));
    return ERR_OK;
}


ErrorCode spuCmdPopr(Processor* spu)
{
    assert(spu != NULL);
    assert(spu->stack.data != NULL);
    assert(spu->call_stack.data != NULL);
    assert(spu->bytecode.data != NULL);

    int register_id = spu->bytecode.data[spu->ip++];
    if (register_id >= (int)REGISTER_COUNT || register_id < 0)
        return ERR_INVALID_REGISTER;

    int value = 0;
    STACK_ERR(stackPop(&spu->stack, &value));
    spu->registers[register_id] = value;
    return ERR_OK;
}


ErrorCode spuCmdPushm(Processor* spu)
{
    assert(spu != NULL);
    assert(spu->stack.data != NULL);
    assert(spu->call_stack.data != NULL);
    assert(spu->bytecode.data != NULL);

    int index = spu->registers[spu->ip++];
    if (index >= (int)RAM_SIZE || index < 0)
        return ERR_INVALID_MEMORY_ACCESS;

    STACK_ERR(stackPush(&spu->stack, spu->ram[index]));
    return ERR_OK;
}


ErrorCode spuCmdPopm(Processor* spu)
{
    assert(spu != NULL);
    assert(spu->stack.data != NULL);
    assert(spu->call_stack.data != NULL);
    assert(spu->bytecode.data != NULL);

    int index = spu->registers[spu->ip++];
    if (index >= (int)RAM_SIZE || index < 0)
        return ERR_INVALID_MEMORY_ACCESS;

    STACK_ERR(stackPop(&spu->stack, &spu->ram[index]));
    return ERR_OK;
}
