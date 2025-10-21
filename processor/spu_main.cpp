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
    REPORT_AND_RETURN(err, &spu);

    err = parseArguments(&spu, argc, argv);
    REPORT_AND_RETURN(err, &spu);

    err = loadByteCode(&spu);
    REPORT_AND_RETURN(err, &spu);

    err = runProcessor(&spu);
    REPORT_AND_RETURN(err, &spu);

    spuDtor(&spu);
    return 0;
}
