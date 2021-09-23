#ifndef _FORTH_PROGRAM_H_
#define _FORTH_PROGRAM_H_

#include "base_types.h"

#include <stdio.h>
#include <stdlib.h>

#include "forth_instruction.h"

typedef struct ForthFunction {
    Instruction *instructions;
    u64 count;
    u64 size;
} ForthFunction;

ForthFunction InitializeFunction(u64 size);


void FreeFunction(ForthFunction *function);
#endif