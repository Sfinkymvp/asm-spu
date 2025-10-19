#ifndef _SPU_INPUT_H_
#define _SPU_INPUT_H_


#include "spu_data.h"


ErrorCode loadByteCode(Processor* spu, const char* input_filename);


ErrorCode createBuffer(Processor* spu);


ErrorCode readInstructions(Processor* spu, FILE* in);


ErrorCode parseArguments(Processor* spu, int argc, const char** argv);


void parseInputFile(int argc, const char** argv, Processor* spu, int* index);


#endif // _SPU_INPUT_H_
