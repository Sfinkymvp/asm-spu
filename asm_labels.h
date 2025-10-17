#ifndef _ASM_LABELS_H_
#define _ASM_LABELS_H_


#include "asm_data.h"


ErrorCode labelTableExpand(LabelTable* label_table);


int getLabelAddress(LabelTable* label_table, const char* label_name);


void addLabel(Label* new_label, const char* label_name, int address);


#endif // _ASM_LABELS_H_
