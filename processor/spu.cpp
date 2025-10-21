#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <unistd.h>


#include "spu.h"
#include "spu_data.h"
#include "spu_error.h"


ErrorCode runProcessor(Processor* spu)
{
    ASSERT_SPU(spu);

    ErrorCode err = spuVerify(spu);

    if (err != ERR_OK)
        return err;

    spu->ip = 0;
    while (spu->ip++ < (int)spu->bytecode.capacity) {
        err = commands[spu->bytecode.data[spu->ip - 1]](spu);
        if (err == ERR_EXIT)
            return ERR_OK;
        if (err != ERR_OK)
            break;
    }

    return err;
}


ErrorCode spuCmdHlt(Processor* spu)
{
    ASSERT_SPU(spu);

    return ERR_EXIT;
}


ErrorCode spuCmdPush(Processor* spu)
{
    ASSERT_SPU(spu);

    int value = spu->bytecode.data[spu->ip++];
    CHECK_OK(spu, stackPush(&spu->stack, value));

    return ERR_OK;
}


ErrorCode spuCmdArithmetic(Processor* spu)
{
    ASSERT_SPU(spu);

    int value1 = 0;
    int value2 = 0;
    CHECK_OK(spu, stackPop(&spu->stack, &value2));
    CHECK_OK(spu, stackPop(&spu->stack, &value1));

    return spuExecArithmetic(spu, value1, value2, (Instruction)spu->bytecode.data[spu->ip - 1]);
}


ErrorCode spuExecArithmetic(Processor* spu, int value1, int value2, Instruction instruction)
{
    ASSERT_SPU(spu);

    switch (instruction) {
        case CMD_ADD: CHECK_OK(spu, stackPush(&spu->stack, value1 + value2)); break;
        case CMD_SUB: CHECK_OK(spu, stackPush(&spu->stack, value1 - value2)); break;
        case CMD_MUL: CHECK_OK(spu, stackPush(&spu->stack, value1 * value2)); break;
        case CMD_DIV: CHECK_OK(spu, stackPush(&spu->stack, value1 / value2)); break;
        default:      return ERR_INVALID_INSTRUCTION;
    }

    return ERR_OK;
}


ErrorCode spuCmdSqrt(Processor* spu)
{
    ASSERT_SPU(spu);

    int value = 0;
    CHECK_OK(spu, stackPop(&spu->stack, &value));
    CHECK_OK(spu, stackPush(&spu->stack, (int)sqrt(value)));

    return ERR_OK;
}


ErrorCode spuCmdIn(Processor* spu)
{
    ASSERT_SPU(spu);

    int value = 0; 
    printf("Enter number: ");
    if (scanf("%d", &value) != 1)
        return ERR_INVALID_ENTER;

    CHECK_OK(spu, stackPush(&spu->stack, value));
    return ERR_OK;
}


ErrorCode spuCmdOut(Processor* spu)
{
    ASSERT_SPU(spu);

    int value = 0;
    CHECK_OK(spu, stackPop(&spu->stack, &value));

    printf("OUT: %d\n", value);
    return ERR_OK;
}


ErrorCode spuCmdJump(Processor* spu)
{
    ASSERT_SPU(spu);

    if (spu->bytecode.data[spu->ip - 1] != CMD_JMP) {
        int value1 = 0;
        int value2 = 0;
        CHECK_OK(spu, stackPop(&spu->stack, &value2));
        CHECK_OK(spu, stackPop(&spu->stack, &value1));

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
    ASSERT_SPU(spu);

    CHECK_OK(spu, stackPush(&spu->call_stack, spu->ip + 1));
    spu->ip = (size_t)spu->bytecode.data[spu->ip];

    return ERR_OK;
}


ErrorCode spuCmdRet(Processor* spu)
{
    ASSERT_SPU(spu);

    int new_index = 0;
    CHECK_OK(spu, stackPop(&spu->call_stack, &new_index));
    spu->ip = new_index;

    return ERR_OK;
}


ErrorCode spuCmdPushr(Processor* spu)
{
    ASSERT_SPU(spu);

    int register_id = spu->bytecode.data[spu->ip++];
    if (register_id >= (int)REGISTER_COUNT || register_id < 0)
        return ERR_INVALID_REGISTER;

    CHECK_OK(spu, stackPush(&spu->stack, spu->registers[register_id]));
    return ERR_OK;
}


ErrorCode spuCmdPopr(Processor* spu)
{
    ASSERT_SPU(spu);

    int register_id = spu->bytecode.data[spu->ip++];
    if (register_id >= (int)REGISTER_COUNT || register_id < 0)
        return ERR_INVALID_REGISTER;

    int value = 0;
    CHECK_OK(spu, stackPop(&spu->stack, &value));
    spu->registers[register_id] = value;
    return ERR_OK;
}


ErrorCode spuCmdPushm(Processor* spu)
{
    ASSERT_SPU(spu);

    int index = spu->registers[spu->bytecode.data[spu->ip++]];
    if (index >= (int)RAM_SIZE || index < 0)
        return ERR_INVALID_MEMORY_ACCESS;

    CHECK_OK(spu, stackPush(&spu->stack, spu->ram[index]));
    return ERR_OK;
}


ErrorCode spuCmdPopm(Processor* spu)
{
    ASSERT_SPU(spu);

    int index = spu->registers[spu->bytecode.data[spu->ip++]];
    if (index >= (int)RAM_SIZE || index < 0)
        return ERR_INVALID_MEMORY_ACCESS;

    CHECK_OK(spu, stackPop(&spu->stack, &spu->ram[index]));
    return ERR_OK;
}


ErrorCode spuCmdDraw(Processor* spu)
{
    ASSERT_SPU(spu);

    static bool first_draw = true;

    unsigned int sleep_time_ms = (unsigned int)spu->bytecode.data[spu->ip++];

    if (sleep_time_ms > 0)
        usleep(sleep_time_ms * 1000);

    if (first_draw) {
        system("clear");
        first_draw = false;
    } else
        printf("\033[H");

    for (size_t index = 0; index < RAM_SIZE; index++) {
        if (index % DRAW_WIDTH == 0 && index != 0)
            printf("\n");
        printf("%c", spu->ram[index]);
    }
    printf("\n");

    return ERR_OK;
}
