#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


#include "asm_labels.h"
#include "asm_data.h"


ErrorCode labelTableExpand(LabelTable* label_table)
{
    assert(label_table != NULL);
    assert(label_table->labels != NULL);

    printf("size of label table: %zu\n", label_table->capacity);
    void* temp = realloc(label_table->labels, 2 * label_table->capacity * sizeof(Label));
    if (temp == NULL)
        return ERR_OUT_OF_MEMORY;

    label_table->labels = (Label*)temp;
    label_table->capacity *= 2;
    return ERR_OK;
}


int getLabelAddress(LabelTable* label_table, const char* label_name)
{
    assert(label_table != NULL);
    assert(label_table->labels != NULL);
    assert(label_name != NULL);

    for (size_t index = 0; index < label_table->count; index++)
        if (strcmp(label_table->labels[index].name, label_name) == 0)
            return label_table->labels[index].address;

    printf("return wait label label with string: %s\n", label_name);
    return WAIT_LABEL;
}


void addLabel(Label* new_label, const char* label_name, int address)
{
    assert(new_label != NULL);
    assert(label_name != NULL);

    strncpy(new_label->name, label_name, LABEL_NAME_MAX - 1);
    new_label->name[LABEL_NAME_MAX - 1] = '\0';
    printf("address: %d\n", address);
    new_label->address = address;
}
