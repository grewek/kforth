#ifndef _FORTH_PROGRAM_H_
#define _FORTH_PROGRAM_H_

#include "base_types.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "forth_instruction.h"

typedef struct ForthFunction {
    Instruction *instructions;
    u64 count;
    u64 size;
} ForthFunction;

ForthFunction InitializeFunction(u64 size);

Instruction *GetNextFreeInstructionSlot(ForthFunction *function);

void AddVoidInstruction(ForthFunction *function, Operation operation);
void AddIntInstruction(ForthFunction *function, Operation operation, i32 value);
void AddStringInstruction(ForthFunction *function, Operation operation, char *buffer, u32 size);
void AddBranchInstructions(ForthFunction *function, u32 targetTrue, u32 targetFalse);
void AddJumpInstruction(ForthFunction *function, Operation operation, u32 target);


void FreeFunction(ForthFunction *function);
#endif