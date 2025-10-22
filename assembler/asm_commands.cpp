#include <stdio.h>
#include <assert.h>


#include "asm_commands.h"
#include "asm_data.h"
#include "asm_labels.h"
#include "asm.h"
#include "asm_utils.h"
#include "asm_hash.h"


ErrorCode asmCmdNoArgs(AssemblyData* asmdata, CommandInfo* cmd_info, char* line)
{
    ASSERT_ASM(asmdata);
    assert(cmd_info != NULL);
    assert(line != NULL);

    asmdata->code.data[asmdata->ip++] = cmd_info->code;

    return ERR_OK;
}


ErrorCode asmCmdOneArg(AssemblyData* asmdata, CommandInfo* cmd_info, char* line)
{
    ASSERT_ASM(asmdata);
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
    ASSERT_ASM(asmdata);
    assert(cmd_info != NULL);
    assert(line != NULL);

    Label label = {NULL, 0, 0};
    CHECK_OK(getWord(&label.name, line, 2));
    label.name += 1;

    assert(label.name != NULL);

    label.hash = hashDjb2((const unsigned char*)label.name); 
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


ErrorCode asmCmdPush(AssemblyData* asmdata, CommandInfo* cmd_info, char* line)
{
    ASSERT_ASM(asmdata);
    assert(cmd_info != NULL);
    assert(line != NULL);

    char* operand = NULL;
    CHECK_OK(getWord(&operand, line, 2));

    ErrorCode err = processRegisterOperand(asmdata, regs, operand, CMD_PUSHR);
    if (err == ERR_OK)
        return ERR_OK;

    err = processRegisterOperand(asmdata, memory_regs, operand, CMD_PUSHM);
    if (err == ERR_OK)
        return ERR_OK;

    long value = 0;
    CHECK_OK(getNumber(&value, operand));
    asmdata->code.data[asmdata->ip++] = CMD_PUSH;
    asmdata->code.data[asmdata->ip++] = (int)value;
    return ERR_OK;
}


ErrorCode asmCmdPop(AssemblyData* asmdata, CommandInfo* cmd_info, char* line)
{
    ASSERT_ASM(asmdata);
    assert(cmd_info != NULL);
    assert(line != NULL);

    char* operand = NULL;
    CHECK_OK(getWord(&operand, line, 2));

    ErrorCode err = processRegisterOperand(asmdata, regs, operand, CMD_POPR);
    if (err == ERR_OK)
        return ERR_OK;

    err = processRegisterOperand(asmdata, memory_regs, operand, CMD_POPM);
    if (err == ERR_OK)
        return ERR_OK;

    return ERR_INVALID_OPERAND;
}


ErrorCode processRegisterOperand(AssemblyData* asmdata, RegisterInfo* registers,
                                 const char* operand, Instruction instruction)
{
    ASSERT_ASM(asmdata);
    assert(registers != NULL);
    assert(operand != NULL);

    Register reg = RESERVED;
    ErrorCode err = getRegister(registers, &reg, operand);
    if (err == ERR_OK) {
        asmdata->code.data[asmdata->ip++] = instruction;
        asmdata->code.data[asmdata->ip++] = reg;
        return ERR_OK;
    }

    return ERR_INVALID_REGISTER;
}
