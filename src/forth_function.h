#ifndef _FORTH_PROGRAM_H_
#define _FORTH_PROGRAM_H_

#include <stdio.h>
#include <stdlib.h>

#include "base_types.h"

typedef enum Operation {
    PUSH,
    POP,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    OUTPUT,
    LESSTHAN,
    GREATERTHAN,
    EQUAL,
    SWAP,
    DUP,
    SUBROUTINE,
    WORD,
} Operation;

typedef enum ForthFunctionError {
    FORTH_FUNCTION_ALLOCATION_FAILED,
} ForthFunctionError;

typedef struct Instruction {
    Operation operation;
    
    union {
        f32 value;
        
        struct Name {
            u64 size;
            char *name;
        } name;

    } valueType;
    
} Instruction;

typedef struct ForthFunction {
    Instruction *instructions;
    u64 count;
    u64 size;
} ForthFunction;

ForthFunction InitializeFunction(u64 size);

//void RunFunction(ForthFunction *function);
void FreeFunction(ForthFunction *function);
#endif