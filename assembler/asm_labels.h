#ifndef _ASM_LABELS_H_
#define _ASM_LABELS_H_


#include "asm_data.h"


ErrorCode defineLabel(AssemblyData* asmdata, char* line);


ErrorCode processLabelFixups(AssemblyData* asmdata);


int getLabelAddress(const LabelTable* label_table, const Label* label);


void addLabel(Label* dest_label, const Label* label);


#endif // _ASM_LABELS_H_
