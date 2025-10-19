#ifndef _SPU_H_
#define _SPU_H_


#include "spu_data.h"


#define STACK_ERR(result) \
    ((result) == SUCCESS ? ERR_OK : ERR_FILE_OPEN)


ErrorCode executeProcessor(Processor* spu);


ErrorCode executeInstruction(Processor* spu, size_t* index);


ErrorCode spuFunc(Processor* spu, size_t* index);


ErrorCode spuCall(Processor* spu, size_t* index);


ErrorCode spuRet(Processor* spu, size_t* index);


ErrorCode spuStack(Processor* spu, size_t* index);


ErrorCode spuPushValue(Processor* spu, size_t* index);


ErrorCode spuPushRegister(Processor* spu, size_t* index);


ErrorCode spuPopRegister(Processor* spu, size_t* index);


ErrorCode spuArithmetic(Processor* spu, size_t* index);


ErrorCode spuPushArithmetic(Processor* spu, int value1, int value2, Instruction instruction);


ErrorCode spuSqrt(Processor* spu, size_t* index);


ErrorCode spuJump(Processor* spu, size_t* index);


bool comparator(int value1, int value2, Instruction instruction);


ErrorCode spuOut(Processor* spu, size_t* index);


ErrorCode spuIn(Processor* spu, size_t* index);


#endif // _SPU_H_

