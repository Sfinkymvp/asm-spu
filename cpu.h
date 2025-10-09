#ifndef _CPU_H_
#define _CPU_H_


#include "cpu_data.h"


#define STACK_ERR(result) \
    ((result) == SUCCESS ? ERR_OK : ERR_FILE_OPEN)


ErrorCode executeProcessor(Processor* cpu);


ErrorCode executeInstruction(Processor* cpu, size_t* index);


ErrorCode cpuStack(Processor* cpu, size_t* index);


ErrorCode cpuPushValue(Processor* cpu, size_t* index);


ErrorCode cpuPushRegister(Processor* cpu, size_t* index);


ErrorCode cpuPopRegister(Processor* cpu, size_t* index);


ErrorCode cpuArithmetic(Processor* cpu, size_t* index);


ErrorCode cpuPushArithmetic(Processor* cpu, int value1, int value2, Instruction instruction);


ErrorCode cpuSqrt(Processor* cpu, size_t* index);


ErrorCode cpuJump(Processor* cpu, size_t* index);


bool comparator(int value1, int value2, Instruction instruction);


ErrorCode cpuOut(Processor* cpu, size_t* index);


ErrorCode cpuIn(Processor* cpu, size_t* index);


#endif // _CPU_H_

