#ifndef _ASM_DATA_H_
#define _ASM_DATA_H_


#include "instructions.h"


const size_t START_CAPACITY = 16;


const size_t LABEL_NAME_MAX = 16;
#define LABEL_LEN_MAX_STR "15"


const int WAIT_LABEL = -1;


extern const char* error_messages[];


#define REPORT_AND_RETURN(err, data_ptr)        \
    do {                                        \
        if ((err) != ERR_OK) {                  \
            printError(err);                    \
            assemblerDtor(data_ptr);            \
            return (err);                       \
        }                                       \
    } while (0)                                 


typedef enum {
    ERR_OK = 0,
    ERR_NOP,
    ERR_FILE_OPEN,
    ERR_FILE_READ,
    ERR_FILE_WRITE,
    ERR_FILE_CLOSE,
    ERR_INVALID_CMD_ARGUMENT,
    ERR_INVALID_INSTRUCTION,
    ERR_INVALID_REGISTER,
    ERR_INVALID_LABEL,
    ERR_OUT_OF_MEMORY
} ErrorCode;


typedef struct {
    const char* input_file;
    const char* output_file;
} Arguments;


typedef struct {
    char name[LABEL_NAME_MAX];
    int address;
} Label;


typedef struct {
    Label* labels;
    size_t count;
    size_t capacity;
} LabelTable;


typedef struct {
    int* data;
    size_t instruction_count;
    size_t capacity;
} ByteCode;


typedef struct {
    Arguments args;
    char* buffer;
    ByteCode code;
    LabelTable label_table;
    LabelTable refs_table;
} AssemblyData;


#endif // _ASM_DATA_H_ 
