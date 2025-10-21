#ifndef _SPU_H_
#define _SPU_H_


#include "spu_data.h"


ErrorCode runProcessor(Processor* spu);


ErrorCode spuCmdHlt(Processor* spu);


ErrorCode spuCmdPush(Processor* spu);


ErrorCode spuCmdArithmetic(Processor* spu);


ErrorCode spuExecArithmetic(Processor* spu, int value1, int value2, Instruction instruction);


ErrorCode spuCmdSqrt(Processor* spu);


ErrorCode spuCmdIn(Processor* spu);


ErrorCode spuCmdOut(Processor* spu);


ErrorCode spuCmdJump(Processor* spu);


bool spuJumpCondition(int value1, int value2, Instruction instruction);


ErrorCode spuCmdCall(Processor* spu);


ErrorCode spuCmdRet(Processor* spu);


ErrorCode spuCmdPushr(Processor* spu);


ErrorCode spuCmdPopr(Processor* spu);


ErrorCode spuCmdPushm(Processor* cpu);


ErrorCode spuCmdPopm(Processor* cpu);


ErrorCode spuCmdDraw(Processor* cpu);


#endif // _SPU_H_

