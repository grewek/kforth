#include "forth_evaluator.h"

void RunFunction(ForthFunction *runningProgram, HashMap *availableFunctions, Stack *stack) {
    //NOTE: This can and probably will blow up the stack...but is there a way to call a function in
    //      a interpreter that does not rely on recursing ? I need to do some research here...
    for(u64 i = 0; i < runningProgram->count; i++) {
        struct Name *funcKey = HandleOperation(&runningProgram->instructions[i], stack);

        if(funcKey) {
            ForthFunction *callee = GetValue(availableFunctions, funcKey->name, funcKey->size);
            RunFunction(callee, availableFunctions, stack);
        }
    }
}