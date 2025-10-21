#ifndef _ASM_COMMANDS_H_
#define _ASM_COMMANDS_H_


#include "asm_data.h"


ErrorCode asmCmdNoArgs(AssemblyData* asmdata, CommandInfo* cmd_info, char* line);


ErrorCode asmCmdOneArg(AssemblyData* asmdata, CommandInfo* cmd_info, char* line);


ErrorCode asmCmdJump(AssemblyData* asmdata, CommandInfo* cmd_info, char* line);


ErrorCode asmCmdRegCommon(AssemblyData* asmdata, RegisterInfo* reg_table, CommandInfo* cmd_info, char* line);


ErrorCode asmCmdRegs(AssemblyData* asmdata, CommandInfo* cmd_info, char* line);


ErrorCode asmCmdMemoryRegs(AssemblyData* asmdata, CommandInfo* cmd_info, char* line);


#endif // _ASM_COMMANDS_H_
