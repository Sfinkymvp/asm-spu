#ifndef _CPU_H_
#define _CPU_H_


#include "cpu_data.h"


ErrorCode executeProcessor(Processor* cpu);


ErrorCode executeInstruction(Processor* cpu, Instruction instruction, size_t* index);


StackError cpuPushValue(Processor* cpu, size_t* index);


StackError cpuAdd(Processor* cpu, size_t* index);


StackError cpuSub(Processor* cpu, size_t* index);


StackError cpuDiv(Processor* cpu, size_t* index);


StackError cpuMul(Processor*cpu, size_t* index);


StackError cpuSqrt(Processor*cpu, size_t* index);
#endif // _CPU_H_

