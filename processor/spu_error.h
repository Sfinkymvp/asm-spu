#ifndef _SPU_ERROR_H_
#define _SPU_ERROR_H_


#include "spu_data.h"


extern const char* error_messages[];


#define ASSERT_SPU(spu)                              \
    assert(spu != NULL);                             \
    assert(spu->stack.data != NULL);                 \
    assert(spu->call_stack.data != NULL);            \
    assert(spu->bytecode.data != NULL);


#ifdef DEBUG

#define CHECK_OK(spu, function_call)                 \
    do {                                             \
        StackError _err = (function_call);           \
        if (_err != SUCCESS) {                       \
            stackDump(&spu->stack, _err);            \
            stackDump(&spu->call_stack, _err);       \
            return (ERR_STACK);                      \
        }                                            \
    } while (0)

#else

#define CHECK_OK(spu, function_call)                 \
    do {                                             \
        if ((function_call) != SUCCESS)              \
            return (ERR_STACK);                      \
    } while (0)

#endif // DEBUG 


#define RETURN_IF_ERROR(_err, spu)                   \
    do {                                             \
        if ((_err) != ERR_OK) {                      \
            spuDtor(spu);                            \
            printError(_err);                        \
            return (_err);                           \
        }                                            \
    } while (0)


void initializeErrorMessages();


ErrorCode spuVerify(Processor* spu);


void printError(ErrorCode err);


#endif // _SPU_ERROR_H_
