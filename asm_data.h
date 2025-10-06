#ifndef _ASM_DATA_H_
#define _ASM_DATA_H_


#include "instructions.h"


const size_t MAX_INSTRUCTION_LEN = 4;


const size_t START_CAPACITY = 16;


extern const char* error_messages[];


#define REPORT_AND_RETURN(error_code, data_ptr) \
    do {                                        \
        if ((error_code) != ERR_OK) {           \
            printError(error_code);             \
            destroyData(data_ptr);              \
            return (error_code);                \
        }                                       \
    } while (0)                                 


typedef enum {
    ERR_OK = 0,
    ERR_FILE_OPEN,
    ERR_FILE_READ,
    ERR_FILE_WRITE,
    ERR_FILE_CLOSE,
    ERR_INVALID_CMD_ARGUMENT,
    ERR_INVALID_INSTRUCTION,
    ERR_OUT_OF_MEMORY
} ErrorCode;


typedef struct {
    const char* input_file;
    const char* output_file;
} Arguments;


typedef struct {
    int* data;
    size_t instruction_count;
    size_t capacity;
} ByteCode;


typedef struct {
    Arguments args;
    char* buffer;
    ByteCode code;
} AssemblyData;


#endif // _ASM_DATA_H_ 
