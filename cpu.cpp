#include <stdio.h>
#include <assert.h>
#include <math.h>


#include "cpu.h"


ErrorCode executeProcessor(Processor* cpu)
{
    assert(cpu != NULL);

    for (size_t index = 0; index < cpu->code.instruction_count; index++) {
        ErrorCode err = ERR_OK;
        err = executeInstruction(cpu, (Instruction)cpu->code.buffer[index], &index);
        if (err != ERR_OK)
            return err;
    }
    
    return ERR_OK;
}


ErrorCode executeInstruction(Processor* cpu, Instruction instruction, size_t* index)
{
    assert(cpu != NULL);
    assert(index != NULL);

    int temp1 = 0;
    size_t register_id = 0;
    switch (instruction) {
        case CMD_HLT:
            break;
        case CMD_PUSH:
            printf("Pushy\n");
            cpuPushValue(cpu, index);
            break;
        case CMD_ADD:  
            cpuAdd(cpu, index);
            break;
        case CMD_SUB: 
            cpuSub(cpu, index);
            break;
        case CMD_DIV:
            cpuDiv(cpu, index);
            break;
        case CMD_MUL: 
            cpuMul(cpu, index);
            break;
        case CMD_SQRT:
            cpuSqrt(cpu, index);
            break;
        case CMD_PUSHR: 
             register_id = (size_t)cpu->code.buffer[++(*index)];
            if (register_id >= REGISTER_COUNT)
                return ERR_INVALID_INPUT;
            temp1 = cpu->registers[register_id];
            stackPush(&cpu->stack, temp1);
            break;
        case CMD_POPR:
            register_id = (size_t)cpu->code.buffer[++(*index)];
            if (register_id >= REGISTER_COUNT)
                return ERR_INVALID_INPUT;
            stackPop(&cpu->stack, &temp1);
            cpu->registers[register_id] = temp1;
            break;
        case CMD_IN:
            printf("Enter number:\n");
            if (scanf("%d", &temp1) != 1)
                return ERR_INVALID_INPUT;
            stackPush(&cpu->stack, temp1);
            break;
        case CMD_JMP:
            cpuJump(cpu, index);
            break;
        case CMD_OUT:
            stackPop(&cpu->stack, &temp1);
            if (printf("%d\n", temp1) < 0)
                return ERR_INVALID_OUTPUT;
            return ERR_OK;
        default:
            return ERR_INVALID_INSTRUCTION;
    }

    return ERR_OK;
}


StackError cpuPushValue(Processor* cpu, size_t* index)
{
    assert(cpu != NULL);
    assert(cpu->stack.data != NULL);
    assert(index != NULL);

    return stackPush(&cpu->stack, cpu->code.buffer[++(*index)]);
}


StackError cpuAdd(Processor* cpu, size_t* index)
{
    assert(cpu != NULL);
    assert(cpu->stack.data != NULL);
    assert(index != NULL);

    int temp1 = 0;
    int temp2 = 0;
    stackPop(&cpu->stack, &temp1);
    stackPop(&cpu->stack, &temp2);
    stackPush(&cpu->stack, temp2 + temp1);

    return SUCCESS;
}


StackError cpuSub(Processor* cpu, size_t* index)
{
    assert(cpu != NULL);
    assert(cpu->stack.data != NULL);
    assert(index != NULL);

    int temp1 = 0;
    int temp2 = 0;
    stackPop(&cpu->stack, &temp1);
    stackPop(&cpu->stack, &temp2);
    stackPush(&cpu->stack, temp2 - temp1);

    return SUCCESS;
}


StackError cpuDiv(Processor* cpu, size_t* index)
{
    assert(cpu != NULL);
    assert(cpu->stack.data != NULL);
    assert(index != NULL);

    int temp1 = 0;
    int temp2 = 0;
    stackPop(&cpu->stack, &temp1);
    stackPop(&cpu->stack, &temp2);
    stackPush(&cpu->stack, temp2 / temp1);

    return SUCCESS;
}


StackError cpuMul(Processor* cpu, size_t* index)
{
    assert(cpu != NULL);
    assert(cpu->stack.data != NULL);
    assert(index != NULL);

    int temp1 = 0;
    int temp2 = 0;
    stackPop(&cpu->stack, &temp1);
    stackPop(&cpu->stack, &temp2);
    stackPush(&cpu->stack, temp2 * temp1);

    return SUCCESS;
}


StackError cpuSqrt(Processor* cpu, size_t* index)
{
    assert(cpu != NULL);
    assert(cpu->stack.data != NULL);
    assert(index != NULL);

    int temp1 = 0;
    stackPop(&cpu->stack, &temp1);
    stackPush(&cpu->stack, (int)sqrt(temp1));

    return SUCCESS;
}


StackError cpuJump(Processor* cpu, size_t* index)
{
    assert(cpu != NULL);
    assert(cpu->stack.data != NULL);
    assert(index != NULL);

    int temp1 = 0;
    int temp2 = 0;
    stackPop(&cpu->stack, &temp1);
    stackPop(&cpu->stack, &temp2);
    if (!comparator(temp2, temp1, (Instruction)cpu->code.buffer))
        return NULL_PTR;
   
    *index = (size_t)cpu->code.buffer[*index + 1] - 1; 
    printf("Index: %zu\n", *index);
    getchar();
    return SUCCESS;
}


bool comparator(int value1, int value2, Instruction instruction)
{
    switch (instruction) {
        case CMD_JMP:
            return true;
        case CMD_JB:
            return value1 < value2;
        case CMD_JBE:
            return value1 <= value2;
        case CMD_JA:
            return value1 > value2;
        case CMD_JAE:
            return value1 >= value2;
        case CMD_JE:
            return value1 == value2;
        case CMD_JNE:
            return value1 != value2;
        default:
            printf("Invalid jump instruction\n");
            return false;
    }
}
