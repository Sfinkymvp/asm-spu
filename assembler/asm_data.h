#ifndef _ASM_DATA_H_
#define _ASM_DATA_H_


#include <stdio.h>


#include "instructions.h"


const size_t START_CAPACITY = 16;
const size_t NAME_MAX_LEN = 30;
#define NAME_MAX_LEN_STR "29"
const int WAIT_LABEL = -1;


#define CHECK_OK(function_call)                 \
    do {                                        \
        ErrorCode _err = (function_call);       \
        if (_err != ERR_OK)                     \
            return (_err);                      \
    } while (0)


typedef enum {
    ERR_OK = 0,
    ERR_FILE_OPEN,
    ERR_FILE_READ,
    ERR_FILE_WRITE,
    ERR_FILE_CLOSE,
    ERR_INVALID_CMD_ARGUMENT,
    ERR_INVALID_INSTRUCTION,
    ERR_INVALID_REGISTER,
    ERR_INVALID_LABEL,
    ERR_INVALID_OPERAND,
    ERR_OUT_OF_MEMORY
} ErrorCode;


const size_t ERROR_TABLE_SIZE = (size_t)ERR_OUT_OF_MEMORY + 1;


typedef struct {
    int count;
    const char** values;
    int pos;
} CmdArgs;


typedef struct {
    const char* input_file;
    const char* output_file;
} Arguments;


typedef struct {
    char* name;
    size_t hash;
    int address;
} Label;


typedef struct {
    Label* labels;
    size_t count;
    size_t capacity;
} LabelTable;


typedef struct {
    int* data;
    size_t capacity;
} ByteCode;


typedef struct {
    Arguments args;
    char* buffer;
    ByteCode code;
    LabelTable label_table;
    LabelTable fixups;
    int ip;
} AssemblyData;


typedef struct CommandInfo CommandInfo;
struct CommandInfo {
    Instruction code;
    const char* name;
    size_t hash;
    ErrorCode (*handler)(AssemblyData*, CommandInfo*, char*);
};


typedef struct {
    Register code;
    const char* name;
    size_t hash;
} RegisterInfo;


extern const char* error_messages[];
extern CommandInfo commands[];
extern const size_t COMMAND_COUNT;
extern RegisterInfo regs[];
extern RegisterInfo memory_regs[];


void initializeErrorMessages();


void printError(ErrorCode err);


#define REPORT_AND_RETURN(err, data_ptr)        \
    do {                                        \
        if ((err) != ERR_OK) {                  \
            printError(err);                    \
            asmDtor(data_ptr);                  \
            return (err);                       \
        }                                       \
    } while (0)                                 


ErrorCode initializeBuffer(AssemblyData* asmdata);


ErrorCode initializeByteCode(AssemblyData* asmdata, size_t start_capacity);


ErrorCode expandByteCode(AssemblyData* asmdata);


ErrorCode initializeLabelTables(AssemblyData* asmdata, size_t start_capacity);


ErrorCode ExpandLabelTable(AssemblyData* asmdata);


ErrorCode asmCtor(AssemblyData* asmdata, int argc, const char** argv);


void asmDtor(AssemblyData* asmdata);


#endif // _ASM_DATA_H_ 
