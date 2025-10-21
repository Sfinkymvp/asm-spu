#include <stdio.h>
#include <assert.h>


#include "asm_commands.h"
#include "asm_data.h"
#include "asm_labels.h"
#include "asm.h"
#include "asm_hash.h"


ErrorCode asmCmdNoArgs(AssemblyData* asmdata, CommandInfo* cmd_info, char* line)
{
    assert(asmdata != NULL);
    assert(asmdata->code.data != NULL);
    assert(cmd_info != NULL);
    assert(line != NULL);

    asmdata->code.data[asmdata->ip++] = cmd_info->code;

    return ERR_OK;
}


ErrorCode asmCmdOneArg(AssemblyData* asmdata, CommandInfo* cmd_info, char* line)
{
    assert(asmdata != NULL);
    assert(asmdata->code.data != NULL);
    assert(cmd_info != NULL);
    assert(line != NULL);

    int value = 0;
    if (sscanf(line, "%*s %d", &value) != 1)
        return ERR_INVALID_INSTRUCTION;

    asmdata->code.data[asmdata->ip++] = cmd_info->code;
    asmdata->code.data[asmdata->ip++] = value;

    return ERR_OK;
}


ErrorCode asmCmdJump(AssemblyData* asmdata, CommandInfo* cmd_info, char* line)
{
    assert(asmdata != NULL);
    assert(asmdata->code.data != NULL);
    assert(cmd_info != NULL);
    assert(line != NULL);

    Label label = {NULL, 0, 0};
    CHECK_OK(getWord(&label.name, line, 2));
    label.name += 1;

    assert(label.name != NULL);

    label.hash = hash_djb2((const unsigned char*)label.name); 
    label.address = getLabelAddress(&asmdata->label_table, &label);
    asmdata->code.data[asmdata->ip++] = cmd_info->code;

    if (label.address != WAIT_LABEL)
        asmdata->code.data[asmdata->ip++] = label.address;
    else {
        label.address = asmdata->ip++;
        addLabel(&asmdata->fixups.labels[asmdata->fixups.count++], &label);
    }

    return ERR_OK;
}


ErrorCode asmCmdRegCommon(AssemblyData* asmdata, RegisterInfo* reg_table, CommandInfo* cmd_info, char* line)
{
    assert(asmdata != NULL);
    assert(asmdata->code.data != NULL);
    assert(regs != NULL);
    assert(cmd_info != NULL);
    assert(line != NULL);

    Register reg = RESERVED;
    CHECK_OK(getRegister(reg_table, &reg, line));

    asmdata->code.data[asmdata->ip++] = cmd_info->code;
    asmdata->code.data[asmdata->ip++] = reg;

    return ERR_OK;
}


ErrorCode asmCmdRegs(AssemblyData* asmdata, CommandInfo* cmd_info, char* line)
{
    assert(asmdata != NULL);
    assert(asmdata->code.data != NULL);
    assert(cmd_info != NULL);
    assert(line != NULL);

    return asmCmdRegCommon(asmdata, regs, cmd_info, line);
}


ErrorCode asmCmdMemoryRegs(AssemblyData* asmdata, CommandInfo* cmd_info, char* line)
{
    assert(asmdata != NULL);
    assert(asmdata->code.data != NULL);
    assert(cmd_info != NULL);
    assert(line != NULL);

    return asmCmdRegCommon(asmdata, memory_regs, cmd_info, line);
}
