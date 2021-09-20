#include <stdio.h>

#include <stdbool.h>
#include <ctype.h>

#include "base_types.h"
#include "lexer.h"
#include "token_list.h"

#define STACK_SIZE 256

//TODO: Seperate into different files for a better overview...

typedef enum {
    PUSH,
    POP,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    OUTPUT,
    LESSTHAN,
    GREATERTHAN,
    EQUAL,
    SWAP,
    DUP,
    SUBROUTINE,
    WORD,
} Operation;

typedef struct {
    //TODO: The stack size should be determined at runtime...
    //TODO: The stack currently can only hold f32 values should it be able to hold different values ?
    f32 values[STACK_SIZE];
    unsigned int stackPtr;
} Stack;


typedef struct  {
    Operation operation;
    f32 value;
} Command;


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
    //TODO: This is just wasted cyles really, but if we want to make it work through UnaryReferenceOperation there is no other way really...
    //TODO cont.: we could make a seperate function that is not based UnaryReference option and save some cycles though...
    return *a;
}

void SwapValues(f32 *a, f32 *b) {
    f32 temp = *a;
    *a = *b;
    *b = temp;
}

typedef f32 (*UnaryOpRef)(f32 *); 
typedef f32 (*BinaryOp)(f32, f32);
typedef void (*BinaryRefOp)(f32 *, f32 *);
//TODO: Delete me...
void UnaryReferenceOperation(Stack *stack, UnaryOpRef op) {
    f32 *a = &stack->values[stack->stackPtr - 1];

    f32 result = op(a);

    Push(stack, result);
}

void BinaryOperation(Stack *stack, BinaryOp op, bool reverse) {
    f32 a = Pop(stack);
    f32 b = Pop(stack);

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


void HandleOperation(Command *cmd, Stack *stack) {
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
        printf("If we encounter a word that isn't part of the forth specs we should try to call it from the program dictionary\n");
        break;

        case DUP:
        UnaryReferenceOperation(stack, DuplicateValue);
        break;
    }
}

typedef struct Program {
    Command instructions[1024];
    i32 size;
} Program;

Program InitializeProgram() {
    Program result = {0};
    return result;
}

TokenType PeekNextToken(Token *tokenList, i32 tokenListSize,  i32 currentPosition) {
    i32 nextTokenPos = currentPosition + 1;

    if(nextTokenPos < tokenListSize) {
        return tokenList[nextTokenPos].tt;
    } else {
        fprintf(stderr, "ERROR: Out of Tokens peeking not possible !");
        exit(1);
    }
}

void ParseForthProgram(Program *prog, TokenList *tokenList) {
    for(u32 i = 0; i < tokenList->_elementCount; i++) {
        
        Token currentToken = PopToken(tokenList);
        switch(currentToken.tt) {
            case T_VALUE:
                prog->instructions[prog->size].operation = PUSH;
                prog->instructions[prog->size].value = atof(currentToken.repr.buffer);
            break;
            
            case T_PLUS:
                prog->instructions[prog->size].operation = PLUS;
            break;
            
            case T_MINUS:
                prog->instructions[prog->size].operation = MINUS;
            break;

            case T_DOT:
                prog->instructions[prog->size].operation = OUTPUT;
            break;
            
            case T_MULTIPLY:
                prog->instructions[prog->size].operation = MULTIPLY;
            break;
            
            case T_DIVIDE:
                prog->instructions[prog->size].operation = DIVIDE;
            break;
            
            case T_LESSTHAN:
                prog->instructions[prog->size].operation = LESSTHAN;
            break;
            
            case T_GREATERTHAN:
                prog->instructions[prog->size].operation = GREATERTHAN;
            break;
            
            case T_EQUAL:
                prog->instructions[prog->size].operation = EQUAL;
            break;
            
            case T_SWAP:
                prog->instructions[prog->size].operation = SWAP;
            break;

            case T_DUP:
                prog->instructions[prog->size].operation = DUP;
            break;

            case T_COLON:
                printf("TODO: Function declaration parsing !\n");
            break;
            
            case T_SEMICOLON:
                printf("TODO: Function definiton end parsing !\n");
            break;

            case T_WORD:
                printf("TODO: Function calling parsin\n");
            break;  
        }

        prog->size += 1;
    }
}


int main(int argc, char **argv) {
    if(argc < 2) {
        fprintf(stderr, "USAGE: kforth [path]\n");
        exit(1);
    }
    const char *path = argv[1];

    TokenList list = GenerateTokenList(path);
    Program forthProgram = InitializeProgram();
    ParseForthProgram(&forthProgram, &list);

    Stack stack = {0};
    for(i32 i = 0; i < forthProgram.size; i++) {
        HandleOperation(&forthProgram.instructions[i], &stack);
    }

    FreeTokenList(&list);
}