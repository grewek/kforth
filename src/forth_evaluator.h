#ifndef _FORTH_EVALUATOR_H_
#define _FORTH_EVALUATOR_H_

#include "forth_function.h"
#include "hash_map.h"
#include "forth_stack.h"

void RunFunction(ForthFunction *runningProgramm, HashMap *availableFunctions, Stack *stack);

#endif