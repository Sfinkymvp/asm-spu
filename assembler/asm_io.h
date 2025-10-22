#ifndef _ASM_INPUT_H_
#define _ASM_INPUT_H_


#include "asm_data.h"


ErrorCode initializeBuffer(AssemblyData* asmdata);


size_t getFileSize(const char* input_filename);


ErrorCode writeNumericByteCode(const AssemblyData* asmdata);


ErrorCode writeBinaryByteCode(const AssemblyData* asmdata);


ErrorCode parseArguments(AssemblyData* asmdata, int argc, const char** argv);


void parseInputFile(AssemblyData* asmdata, CmdArgs* args);


void parseOutputFile(AssemblyData* asmdata, CmdArgs* args);


#endif // _ASM_INPUT_H_
