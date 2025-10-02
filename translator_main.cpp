#include <stdio.h>

#include "translator.h"
#include "translator_input.h"


int main()
{
    char* buffer = initializeBuffer("test1");

    if (!translator(buffer))
        return 1;

    return 0;
}

