#ifndef _ASM_INPUT_H_
#define _ASM_INPUT_H_


#include "asm_data.h"


size_t getFileSize(const char* input_filename);


ErrorCode initializeBuffer(char** buffer, const char* input_filename);


ErrorCode parseArguments(Arguments* args, size_t argc, const char** argv);


void parseInputFile(size_t argc, const char** argv, Arguments* args, size_t* index);


void parseOutputFile(size_t argc, const char** argv, Arguments* args, size_t* index);


#endif // _ASM_INPUT_H_
