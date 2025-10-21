#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>


#include "spu_data.h"
#include "spu.h"
#include "spu_input.h"
#include "spu_error.h"


int main(int argc, const char** argv)
{
    initializeCommands();
    initializeErrorMessages();

    Processor spu = {};

    ErrorCode err = spuCtor(&spu);
    RETURN_IF_ERROR(err, &spu);

    err = parseArguments(&spu, argc, argv);
    RETURN_IF_ERROR(err, &spu);

    err = loadByteCode(&spu);
    RETURN_IF_ERROR(err, &spu);

    err = runProcessor(&spu);
    RETURN_IF_ERROR(err, &spu);

    spuDtor(&spu);
    return 0;
}
