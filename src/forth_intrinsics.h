#ifndef _FORTH_INTRINSICS_H_
#define _FORTH_INTRINSICS_H_

#include <stdio.h>
#include <stdlib.h>

#include "forth_stack.h"
#include "base_types.h"

//TODO: Define all functions that forth provides to the user here.
//Stack Operations
void Push(Stack *stack, f32 value);
f32 Pop(Stack *stack);

//Arithmetic Operations
f32 Plus(f32 a, f32 b);
f32 Minus(f32 a, f32 b);
f32 Multiply(f32 a, f32 b);
f32 Divide(f32 a, f32 b);

//Logic Operations
f32 Equal(f32 a, f32 b);
f32 LessThan(f32 a, f32 b);
f32 GreaterThan(f32 a, f32 b);

//Reference Based Stack Operations
f32 DuplicateValue(f32 *a);
void SwapValues(f32 *a, f32 *b);

#endif