#include "forth_evaluator.h"

void RunFunction(ForthFunction *runningProgram, HashMap *availableFunctions, Stack *stack) {
    //NOTE: This can and probably will blow up the stack...but is there a way to call a function in
    //      a interpreter that does not rely on recursing ? I need to do some research here...
    while(stack->instrPtr < runningProgram->count) {
        ForthCell *funcKey = HandleOperation(&runningProgram->instructions[stack->instrPtr], stack);

        
        if(funcKey && funcKey->ct == CELL_STRING) {
            ForthFunction *callee = GetValue(availableFunctions, funcKey->innerType.string.buffer, funcKey->innerType.string.size);
            u32 oldInstrPtr = stack->instrPtr;
            stack->instrPtr = 0;
            RunFunction(callee, availableFunctions, stack);
            stack->instrPtr = oldInstrPtr;
            stack->instrPtr += 1;
        }
    }
}