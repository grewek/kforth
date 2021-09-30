#ifndef _FORTH_INSTRUCTION_H_
#define _FORTH_INSTRUCTION_H_

#include "forth_cell.h"
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
    DEF_STRING,
    ROTATE,
    JMP,
    JMP_EQ,
} Operation;

typedef struct Instruction {
    Operation operation;
    ForthCell value;
    
} Instruction;

#endif