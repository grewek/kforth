#include "forth_evaluator.h"

void RunFunction(ForthFunction *runningProgram, HashMap *availableFunctions, Stack *stack) {
    //NOTE: This can and probably will blow up the stack...but is there a way to call a function in
    //      a interpreter that does not rely on recursing ? I need to do some research here...
    for(u64 i = 0; i < runningProgram->count; i++) {
        ForthCell *funcKey = HandleOperation(&runningProgram->instructions[i], stack);

        if(funcKey && funcKey->ct == CELL_STRING) {
            ForthFunction *callee = GetValue(availableFunctions, funcKey->innerType.string.buffer, funcKey->innerType.string.size);
            RunFunction(callee, availableFunctions, stack);
        }
    }
}