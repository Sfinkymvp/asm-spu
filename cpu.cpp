#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>


#include "cpu.h"


ErrorCode executeProcessor(Processor* cpu)
{
    assert(cpu != NULL);
    assert(cpu->stack.data != NULL);
    assert(cpu->code.buffer != NULL);

    for (size_t index = 0; index < cpu->code.instruction_count;) {
        ErrorCode err = ERR_OK;
        err = executeInstruction(cpu, &index);
        if (err != ERR_OK) {
            return err;
        }
    }

    return ERR_OK;
}


ErrorCode executeInstruction(Processor* cpu, size_t* index)
{
    assert(cpu != NULL);
    assert(cpu->stack.data != NULL);
    assert(cpu->code.buffer != NULL);
    assert(index != NULL);

    switch (cpu->code.buffer[*index]) {
        case CMD_ADD: 
        case CMD_SUB: 
        case CMD_DIV:
        case CMD_MUL:   return cpuArithmetic(cpu, index);
        case CMD_SQRT:  return cpuSqrt(cpu, index);
        case CMD_PUSH:
        case CMD_PUSHR:
        case CMD_POPR:  return cpuStack(cpu, index);
        case CMD_JMP:
        case CMD_JB:
        case CMD_JBE:
        case CMD_JA:
        case CMD_JAE:
        case CMD_JE:
        case CMD_JNE:   return cpuJump(cpu, index);
        case CMD_IN:    return cpuIn(cpu, index);
        case CMD_OUT:   return cpuOut(cpu, index);
        case CMD_HLT:   return cpuHlt(index);
        default:        printf("default\n"); return ERR_INVALID_INSTRUCTION;
    }
}


ErrorCode cpuStack(Processor* cpu, size_t* index)
{
    assert(cpu != NULL);
    assert(cpu->stack.data != NULL);
    assert(cpu->code.buffer != NULL);
    assert(index != NULL);

    switch ((Instruction)cpu->code.buffer[*index]) {
        case CMD_PUSH:  return STACK_ERR(cpuPushValue(cpu, index));
        case CMD_PUSHR: return STACK_ERR(cpuPushRegister(cpu, index));
        case CMD_POPR:  return STACK_ERR(cpuPopRegister(cpu, index));
        default:        
        printf("INVAL\n");
        return ERR_INVALID_OPERATION;
    }
}


ErrorCode cpuPushValue(Processor* cpu, size_t* index)
{
    assert(cpu != NULL);
    assert(cpu->stack.data != NULL);
    assert(cpu->code.buffer != NULL);
    assert(index != NULL);

    int value = cpu->code.buffer[++(*index)];
    ErrorCode err = STACK_ERR(stackPush(&cpu->stack, value));
    (*index)++;
    return err;
}


ErrorCode cpuPushRegister(Processor* cpu, size_t* index)
{
    assert(cpu != NULL);
    assert(cpu->stack.data != NULL);
    assert(cpu->code.buffer != NULL);
    assert(index != NULL);

    int register_id = cpu->code.buffer[++(*index)];
    if (register_id >= REGISTER_COUNT || register_id < 0)
        return ERR_INVALID_REGISTER;

    int value = cpu->registers[register_id];
    stackPush(&cpu->stack, value);
    (*index)++;

    printf("register RAX: %d\n", cpu->registers[RAX]);
    return ERR_OK;
}


ErrorCode cpuPopRegister(Processor* cpu, size_t* index)
{
    assert(cpu != NULL);
    assert(cpu->stack.data != NULL);
    assert(cpu->code.buffer != NULL);
    assert(index != NULL);

    int register_id = cpu->code.buffer[++(*index)];
    if (register_id >= REGISTER_COUNT || register_id < 0)
        return ERR_INVALID_REGISTER;

    int value = 0;
    stackPop(&cpu->stack, &value);
    cpu->registers[register_id] = value;
    (*index)++;
    return ERR_OK;
}


ErrorCode cpuArithmetic(Processor* cpu, size_t* index)
{
    assert(cpu != NULL);
    assert(cpu->stack.data != NULL);
    assert(cpu->code.buffer != NULL);
    assert(index != NULL);

    int value1 = 0;
    int value2 = 0;
    stackPop(&cpu->stack, &value2);
    stackPop(&cpu->stack, &value1);
   
    Instruction instruction = (Instruction)cpu->code.buffer[(*index)++];
    return STACK_ERR(cpuPushArithmetic(cpu, value1, value2, instruction));
}


ErrorCode cpuPushArithmetic(Processor* cpu, int value1, int value2, Instruction instruction)
{
    switch (instruction) {
        case CMD_ADD: return STACK_ERR(stackPush(&cpu->stack, value1 + value2));
        case CMD_SUB: return STACK_ERR(stackPush(&cpu->stack, value1 - value2));
        case CMD_MUL: return STACK_ERR(stackPush(&cpu->stack, value1 * value2));
        case CMD_DIV: return STACK_ERR(stackPush(&cpu->stack, value1 / value2));
        default:      return ERR_INVALID_INSTRUCTION;
    }
}


ErrorCode cpuSqrt(Processor* cpu, size_t* index)
{
    assert(cpu != NULL);
    assert(cpu->stack.data != NULL);
    assert(cpu->code.buffer != NULL);
    assert(index != NULL);

    int value = 0;
    stackPop(&cpu->stack, &value);
    stackPush(&cpu->stack, (int)sqrt(value));

    (*index)++;
    return ERR_OK;
}


ErrorCode cpuJump(Processor* cpu, size_t* index)
{
    assert(cpu != NULL);
    assert(cpu->stack.data != NULL);
    assert(cpu->code.buffer != NULL);
    assert(index != NULL);

    if (cpu->code.buffer[*index] != CMD_JMP) {
        int value1 = 0;
        int value2 = 0;
        stackPop(&cpu->stack, &value1);
        stackPop(&cpu->stack, &value2);
        if (!comparator(value2, value1, (Instruction)cpu->code.buffer[*index])) {
            *index += 2;
            return ERR_OK;
        }
    }
   
    *index = (size_t)cpu->code.buffer[++(*index)];
    getchar();

    return ERR_OK;
}


bool comparator(int value1, int value2, Instruction instruction)
{
    switch (instruction) {
        case CMD_JMP:  return true;
        case CMD_JB:   return value1 < value2;
        case CMD_JBE:  return value1 <= value2;
        case CMD_JA:   return value1 > value2;
        case CMD_JAE:  return value1 >= value2;
        case CMD_JE:   return value1 == value2;
        case CMD_JNE:  return value1 != value2;
        default:       printf("Invalid jump instruction\n");
                       return false;
    }
}


ErrorCode cpuOut(Processor* cpu, size_t* index)
{
    assert(cpu != NULL);
    assert(cpu->stack.data != NULL);
    assert(cpu->code.buffer != NULL);
    assert(index != NULL);

    int value = 0;
    stackPop(&cpu->stack, &value);

    if (printf("ROOT: %d\n", value) < 0) {
        printf("printf\n");
        return ERR_INVALID_OPERATION;
    }

    (*index)++;
    return ERR_OK;
}


ErrorCode cpuIn(Processor* cpu, size_t* index)
{
    assert(cpu != NULL);
    assert(cpu->stack.data != NULL);
    assert(cpu->code.buffer != NULL);
    assert(index != NULL);

    int value = 0; 
    printf("Enter number:\n");
    if (scanf("%d", &value) != 1) {
        printf("scanf\n");
        return ERR_INVALID_OPERATION;
    }

    stackPush(&cpu->stack, value);

    (*index)++;
    return ERR_OK;
}


ErrorCode cpuHlt(size_t* index)
{
    (*index)++;

    return ERR_OK;
}
