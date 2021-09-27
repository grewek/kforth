#ifndef _FORTH_STACK_H_
#define _FORTH_STACK_H_

#define STACK_SIZE 256

#include <stdbool.h>

#include "forth_cell.h"
#include "forth_instruction.h"
#include "stack_operations.h"
#include "token.h"
#include "base_types.h"

typedef struct {
    //TODO: The stack size should be determined at runtime...
    ForthCell values[STACK_SIZE];
    u32 stackPtr;
} Stack;

void Push(Stack *stack, ForthCell value);
ForthCell Pop(Stack *stack);

//TODO: Should these function pointers be in the intrinsics file or in the stack file ?

typedef void (UnaryOpVoid)(ForthCell);

typedef ForthCell (UnaryOp)(ForthCell);
typedef ForthCell (*UnaryOpRef)(ForthCell *); 

typedef ForthCell (*BinaryOp)(ForthCell, ForthCell);

typedef void (*BinaryRefOp)(ForthCell *, ForthCell *);

typedef void (*TernaryRefOp)(ForthCell *, ForthCell *, ForthCell *);

ForthCell *HandleOperation(Instruction *cmd, Stack *stack);

void UnaryReferenceOperation(Stack *stack, UnaryOpRef op);
void BinaryOperation(Stack *stack, BinaryOp op, bool reverse);
void BinaryReferenceOperation(Stack *stack, BinaryRefOp op);

#endif