#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


#include "spu_data.h"
#include "spu.h"


CommandInfo commands[CMD_TABLE_SIZE] = {};


void initializeCommands() {
    commands[CMD_HLT]   = {CMD_HLT,   spuCmdHlt};
    commands[CMD_PUSH]  = {CMD_PUSH,  spuCmdPush};
    commands[CMD_ADD]   = {CMD_ADD,   spuCmdArithmetic};
    commands[CMD_SUB]   = {CMD_SUB,   spuCmdArithmetic};
    commands[CMD_DIV]   = {CMD_DIV,   spuCmdArithmetic};
    commands[CMD_MUL]   = {CMD_MUL,   spuCmdArithmetic};
    commands[CMD_SQRT]  = {CMD_SQRT,  spuCmdSqrt};
    commands[CMD_IN]    = {CMD_IN,    spuCmdIn};
    commands[CMD_OUT]   = {CMD_OUT,   spuCmdOut};
    commands[CMD_JMP]   = {CMD_JMP,   spuCmdJump};
    commands[CMD_JB]    = {CMD_JB,    spuCmdJump};
    commands[CMD_JBE]   = {CMD_JBE,   spuCmdJump};
    commands[CMD_JA]    = {CMD_JA,    spuCmdJump};
    commands[CMD_JAE]   = {CMD_JAE,   spuCmdJump};
    commands[CMD_JE]    = {CMD_JE,    spuCmdJump};
    commands[CMD_JNE]   = {CMD_JNE,   spuCmdJump};
    commands[CMD_CALL]  = {CMD_CALL,  spuCmdCall};
    commands[CMD_RET]   = {CMD_RET,   spuCmdRet};
    commands[CMD_PUSHR] = {CMD_PUSHR, spuCmdPushr};
    commands[CMD_POPR]  = {CMD_POPR,  spuCmdPopr};
    commands[CMD_PUSHM] = {CMD_PUSHM, spuCmdPushm};
    commands[CMD_POPM]  = {CMD_POPM,  spuCmdPopm};
}
