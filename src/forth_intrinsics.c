#include "forth_intrinsics.h"

void Push(Stack *stack, f32 value) {
    stack->values[stack->stackPtr] = value;
    stack->stackPtr++;
}

f32 Pop(Stack *stack) {
    
    if(stack->stackPtr > 0) {
        stack->stackPtr--;
    } else {
        fprintf(stderr, "ERROR: Stack underflow\n");
        exit(1); //TODO: Think of a way of catching the error and handling it gracefully ?
    }

    f32 result = stack->values[stack->stackPtr];

    return result;
}

f32 Plus(f32 a, f32 b) {
    return a + b;
}

f32 Minus(f32 a, f32 b) {
    return a - b;
}

f32 Multiply(f32 a, f32 b) {
    return a * b;
}

f32 Divide(f32 a, f32 b) {
    return a / b;
}

f32 Equal(f32 a, f32 b) {
    return a == b ? -1.0 : 0.0;
}

f32 LessThan(f32 a, f32 b) {
    return a < b ? -1.0 : 0.0;
}

f32 GreaterThan(f32 a, f32 b) {
    //NOTE: According to the forth specs all values other than 0 are truthy values, but it seems like that -1.0 is the standard
    //      value...
    return a > b ? -1.0 : 0.0;
}

f32 DuplicateValue(f32 *a) {
    return *a;
}

void SwapValues(f32 *a, f32 *b) {
    f32 temp = *a;
    *a = *b;
    *b = temp;
}