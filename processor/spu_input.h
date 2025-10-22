#ifndef _SPU_INPUT_H_
#define _SPU_INPUT_H_


#include "spu_data.h"


ErrorCode loadNumericByteCode(Processor* spu);


ErrorCode loadBinaryByteCode(Processor* spu);


ErrorCode createBuffer(Processor* spu);


ErrorCode readByteCode(Processor* spu, FILE* in);


ErrorCode parseArguments(Processor* spu, int argc, const char** argv);


void parseInputFile(Processor* spu, CmdArgs* args);


#endif // _SPU_INPUT_H_
