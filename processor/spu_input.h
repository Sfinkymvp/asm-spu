#ifndef _SPU_INPUT_H_
#define _SPU_INPUT_H_


#include "spu_data.h"


ErrorCode loadByteCode(ByteCode* code, const char* input_filename);


ErrorCode createBuffer(ByteCode* code, size_t size);


ErrorCode readInstructions(ByteCode* code, FILE* in);


ErrorCode parseArguments(Arguments* args, size_t argc, const char** argv);


void parseInputFile(size_t argc, const char** argv, Arguments* args, size_t* index);


#endif // _SPU_INPUT_H_
