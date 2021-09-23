#ifndef _FORTH_STACK_H_
#define _FORTH_STACK_H_

#define STACK_SIZE 256

#include <stdbool.h>

#include "forth_instruction.h"
#include "forth_intrinsics.h"
#include "forth_stack.h"

#include "token.h"
#include "base_types.h"
typedef struct {
    //TODO: The stack size should be determined at runtime...
    //TODO: The stack currently can only hold f32 values should it be able to hold different values ?
    f32 values[STACK_SIZE];
    unsigned int stackPtr;
} Stack;

void Push(Stack *stack, f32 value);
f32 Pop(Stack *stack);

//TODO: Should these function pointers be in the intrinsics file or in the stack file ?
typedef f32 (*UnaryOpRef)(f32 *); 
typedef f32 (*BinaryOp)(f32, f32);
typedef void (*BinaryRefOp)(f32 *, f32 *);

struct Name *HandleOperation(Instruction *cmd, Stack *stack);

void UnaryReferenceOperation(Stack *stack, UnaryOpRef op);
void BinaryOperation(Stack *stack, BinaryOp op, bool reverse);
void BinaryReferenceOperation(Stack *stack, BinaryRefOp op);

#endif