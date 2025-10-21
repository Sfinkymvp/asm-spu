#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


#include "spu_data.h"
#include "spu_error.h"
#include "spu.h"


CommandHandler commands[CMD_TABLE_SIZE] = {};


void initializeCommands() {
    commands[CMD_HLT]   = spuCmdHlt;
    commands[CMD_PUSH]  = spuCmdPush;
    commands[CMD_ADD]   = spuCmdArithmetic;
    commands[CMD_SUB]   = spuCmdArithmetic;
    commands[CMD_DIV]   = spuCmdArithmetic;
    commands[CMD_MUL]   = spuCmdArithmetic;
    commands[CMD_SQRT]  = spuCmdSqrt;
    commands[CMD_IN]    = spuCmdIn;
    commands[CMD_OUT]   = spuCmdOut;
    commands[CMD_JMP]   = spuCmdJump;
    commands[CMD_JB]    = spuCmdJump;
    commands[CMD_JBE]   = spuCmdJump;
    commands[CMD_JA]    = spuCmdJump;
    commands[CMD_JAE]   = spuCmdJump;
    commands[CMD_JE]    = spuCmdJump;
    commands[CMD_JNE]   = spuCmdJump;
    commands[CMD_CALL]  = spuCmdCall;
    commands[CMD_RET]   = spuCmdRet;
    commands[CMD_PUSHR] = spuCmdPushr;
    commands[CMD_POPR]  = spuCmdPopr;
    commands[CMD_PUSHM] = spuCmdPushm;
    commands[CMD_POPM]  = spuCmdPopm;
    commands[CMD_DRAW]  = spuCmdDraw;
}


ErrorCode spuCtor(Processor* spu)
{
    assert(spu != NULL);

    CHECK_OK(spu, stackCtor(&spu->stack, START_STACK_CAPACITY));
    CHECK_OK(spu, stackCtor(&spu->call_stack, START_STACK_CAPACITY));

    return ERR_OK;    
}


ErrorCode spuDtor(Processor* spu)
{
    assert(spu != NULL);

    stackDtor(&spu->stack);
    stackDtor(&spu->call_stack);
    free(spu->bytecode.data);

    return ERR_OK;
}
