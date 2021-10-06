#include "forth_function.h"

ForthFunction InitializeFunction(u64 size) {
    ForthFunction result = {0};
    result.count = 0;
    result.size = size;
    result.instructions = calloc(result.size, sizeof(Instruction));

    if(result.instructions == NULL) {
        fprintf(stderr, "ERROR: Could not reserve memory for forthfunction\n");
        exit(1);
    }

    return result;
}

Instruction *GetNextFreeInstructionSlot(ForthFunction *function) {
    Instruction *result = &function->instructions[function->count];
    assert(function->count < function->size);
    function->count += 1;

    return result;
}

void AddVoidInstruction(ForthFunction *function, Operation operation) {
    assert(function != NULL);

    Instruction *currentInstruction = GetNextFreeInstructionSlot(function);
    currentInstruction->operation = operation;
    currentInstruction->value.ct = CELL_VOID;
}

void AddIntInstruction(ForthFunction *function, Operation operation, i32 value) {
    assert(function != NULL);

    Instruction *currentInstruction = GetNextFreeInstructionSlot(function);
    currentInstruction->operation = operation;
    currentInstruction->value.ct = CELL_INT;
    currentInstruction->value.innerType.integer = value;
}

void AddStringInstruction(ForthFunction *function, Operation operation, char *buffer, u32 size) {
    assert(function != NULL);

    Instruction *currentInstruction = GetNextFreeInstructionSlot(function);
    currentInstruction->operation = operation;
    currentInstruction->value.ct = CELL_STRING;
    currentInstruction->value.innerType.string.buffer = buffer;
    currentInstruction->value.innerType.string.size = size;
}

void AddConditionalGoto(ForthFunction *function) {
    Instruction *currentInstruction = GetNextFreeInstructionSlot(function);
    currentInstruction->operation = CONDITIONAL_GOTO;
    currentInstruction->value.ct = CELL_VOID;
}

void AddUnconditionalGoto(ForthFunction *function) {
    Instruction *currentInstruction = GetNextFreeInstructionSlot(function);
    currentInstruction->operation = UNCONDITIONAL_GOTO;
    currentInstruction->value.ct = CELL_VOID;
}

void InsertRelativePosition(ForthFunction *function, u32 instructionIndex, i32 realtivePosition) {
    Instruction *gotoInstruction = &function->instructions[instructionIndex];
    
    if((gotoInstruction->operation == CONDITIONAL_GOTO || gotoInstruction->operation == UNCONDITIONAL_GOTO) && 
        gotoInstruction->value.ct == CELL_VOID) {
        gotoInstruction->value.ct = CELL_INT;
        gotoInstruction->value.innerType.integer = realtivePosition;
    }
}

void FreeFunction(ForthFunction *function) {
    if(function->instructions != NULL) {
        free(function->instructions);
        function->instructions = NULL;
    }
}