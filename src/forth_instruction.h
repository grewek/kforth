#ifndef _FORTH_INSTRUCTION_H_
#define _FORTH_INSTRUCTION_H_

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

#endif