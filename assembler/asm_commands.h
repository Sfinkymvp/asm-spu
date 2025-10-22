#ifndef _ASM_COMMANDS_H_
#define _ASM_COMMANDS_H_


#include "asm_data.h"


ErrorCode asmCmdNoArgs(AssemblyData* asmdata, CommandInfo* cmd_info, char* line);


ErrorCode asmCmdOneArg(AssemblyData* asmdata, CommandInfo* cmd_info, char* line);


ErrorCode asmCmdJump(AssemblyData* asmdata, CommandInfo* cmd_info, char* line);


ErrorCode asmCmdPush(AssemblyData* asmdata, CommandInfo* cmd_info, char* line);


ErrorCode asmCmdPop(AssemblyData* asmdata, CommandInfo* cmd_info, char* line);


ErrorCode processRegisterOperand(AssemblyData* asmdata, RegisterInfo* registers,
                                 const char* operand, Instruction instruction);


#endif // _ASM_COMMANDS_H_
