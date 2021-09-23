#include "forth_stack.h"

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

void UnaryReferenceOperation(Stack *stack, UnaryOpRef op) {
    f32 *a = &stack->values[stack->stackPtr - 1];

    f32 result = op(a);

    Push(stack, result);
}

void BinaryOperation(Stack *stack, BinaryOp op, bool reverse) {
    f32 b = Pop(stack);
    f32 a = Pop(stack);

    f32 result;
    if(reverse) {
        result = op(b, a);
    } else {
        result = op(a, b);
    }
    
    Push(stack, result);
}

void BinaryReferenceOperation(Stack *stack, BinaryRefOp op) {
    //This fails if there are not enough elements on the stack...
    f32 *refA = &stack->values[stack->stackPtr - 1];
    f32 *refB = &stack->values[stack->stackPtr - 2];

    op(refA, refB);
}

void Output(Stack *stack) {
    f32 a = Pop(stack);

    printf("%f ok\n", a);
}


struct Name *HandleOperation(Instruction *cmd, Stack *stack) {
    switch(cmd->operation) {
        case PUSH: 
        Push(stack, cmd->valueType.value);
        break;
        
        case POP:
        Pop(stack);
        break;
        
        case PLUS:
        BinaryOperation(stack, Plus, false);
        break;

        case MINUS:
        BinaryOperation(stack, Minus, false);
        break;

        case MULTIPLY:
        BinaryOperation(stack, Multiply, false);
        break;

        case DIVIDE:
        BinaryOperation(stack, Divide, false);
        break;

        case EQUAL:
        BinaryOperation(stack, Equal, true);
        break;

        case GREATERTHAN:
        BinaryOperation(stack, GreaterThan, true);
        break;

        case LESSTHAN:
        BinaryOperation(stack, LessThan, true);
        break;

        case SWAP:
        BinaryReferenceOperation(stack, SwapValues);
        break;

        case OUTPUT:
        Output(stack);
        break;

        case SUBROUTINE:
        printf("What todo here ? do we really need this ?\n");
        break;

        case WORD:
            return &cmd->valueType.name;
        break;

        case DUP:
        UnaryReferenceOperation(stack, DuplicateValue);
        break;
    }

    return NULL;
}