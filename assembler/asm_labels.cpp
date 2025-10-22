#include <stdio.h>
#include <string.h>
#include <assert.h>


#include "asm_labels.h"
#include "asm_data.h"
#include "asm_hash.h"


ErrorCode defineLabel(AssemblyData* asmdata, char* line)
{
    ASSERT_ASM(asmdata);
    assert(line != NULL);

    if (asmdata->label_table.count == asmdata->label_table.capacity)
        CHECK_OK(ExpandLabelTable(asmdata));

    Label label = {NULL, 0, 0};
    label.name = line;
    if (*label.name == '\0')
        return ERR_INVALID_LABEL;
     
    label.hash = hashDjb2((const unsigned char*)label.name); 
    label.address = asmdata->ip;
    addLabel(&asmdata->label_table.labels[asmdata->label_table.count++], &label);

    return ERR_OK;
}


ErrorCode processLabelFixups(AssemblyData* asmdata)
{
    ASSERT_ASM(asmdata);

    for (size_t index = 0; index < asmdata->fixups.count; index++) {
        int address = getLabelAddress(&asmdata->label_table,
                                      &asmdata->fixups.labels[index]);

        assert(address != WAIT_LABEL);

        int bytecode_address = asmdata->fixups.labels[index].address;
        asmdata->code.data[bytecode_address] = address;
    }

    return ERR_OK;
}


int getLabelAddress(const LabelTable* label_table, const Label* label)
{
    assert(label_table != NULL);
    assert(label_table->labels != NULL);
    assert(label != NULL);

    for (size_t index = 0; index < label_table->count; index++)
        if (label_table->labels[index].hash == label->hash &&
            strcmp(label_table->labels[index].name, label->name) == 0)
            return label_table->labels[index].address;

    return WAIT_LABEL;
}


void addLabel(Label* dest_label, const Label* label)
{
    assert(dest_label != NULL);
    assert(label != NULL);
    assert(label->name != NULL);

    dest_label->name = label->name;
    dest_label->hash = label->hash;
    dest_label->address = label->address;
}
