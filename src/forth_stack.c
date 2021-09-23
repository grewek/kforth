#include "forth_stack.h"

void Push(Stack *stack, ForthCell value) {
    stack->values[stack->stackPtr] = value;
    stack->stackPtr++;
}

ForthCell Pop(Stack *stack) {
    
    if(stack->stackPtr > 0) {
        stack->stackPtr--;
    } else {
        fprintf(stderr, "ERROR: Stack underflow\n");
        exit(1); //TODO: Think of a way of catching the error and handling it gracefully ?
    }

    ForthCell result = stack->values[stack->stackPtr];

    return result;
}

void UnaryReferenceOperation(Stack *stack, UnaryOpRef op) {
    ForthCell *a = &stack->values[stack->stackPtr - 1];

    ForthCell result = op(a);

    Push(stack, result);
}

void BinaryOperation(Stack *stack, BinaryOp op, bool reverse) {
    ForthCell b = Pop(stack);
    ForthCell a = Pop(stack);

    ForthCell result;
    if(reverse) {
        result = op(b, a);
    } else {
        result = op(a, b);
    }
    
    Push(stack, result);
}

void BinaryReferenceOperation(Stack *stack, BinaryRefOp op) {
    //This fails if there are not enough elements on the stack...
    ForthCell *refA = &stack->values[stack->stackPtr - 1];
    ForthCell *refB = &stack->values[stack->stackPtr - 2];

    op(refA, refB);
}

void Output(Stack *stack) {
    ForthCell a = Pop(stack);

    switch(a.ct) {
        case CELL_INT:
            printf("%d ok\n", a.innerType.integer);
        break;
        case CELL_STRING:
            printf("%s ok\n", a.innerType.string.buffer);
        break;
        default:
        break;
    }
    
}


ForthCell *HandleOperation(Instruction *cmd, Stack *stack) {
    switch(cmd->operation) {
        case PUSH:
        Push(stack, cmd->value);
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
            return &cmd->value;
        break;

        case DUP:
        UnaryReferenceOperation(stack, DuplicateValue);
        break;

        case DEF_STRING:
        printf("Ok now what...?!");
        break;
    }

    return NULL;
}