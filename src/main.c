#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/stat.h>

#define STACK_SIZE 256

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t i8;

typedef float f32;
typedef float f64;

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
} Operation;

typedef struct {
    //TODO: The stack size should be determined at runtime...
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
        stack->stackPtr = 0;
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
    return a > b ? -1.0 : 0.0;
}

typedef f32 (*UnaryOp)(f32);
typedef f32 (*BinaryOp)(f32, f32);

void UnaryOperation(Stack *stack, UnaryOp op) {
    f32 a = Pop(stack);

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

        case OUTPUT:
        Output(stack);
        break;

        /*case DUMP:
        Dump(stack);
        break;*/

        default: break;
    }
}

typedef enum {
    T_VALUE,
    T_OPERATOR,
    T_PLUS,
    T_MINUS,
    T_DOT,
    T_MULTIPLY,
    T_DIVIDE,
    T_EQUAL,
    T_LESSTHAN,
    T_GREATERTHAN,
} TokenType;

typedef struct  {
    TokenType tt;

    i32 start;
    i32 end;
    i32 length;

    union {
        char *buffer;
        char symbol;
    } repr;
    

} Token;

char Peek(const char *buffer, i32 *currentPosition, i32 size) {
    if(*currentPosition < size) {
        return buffer[*currentPosition + 1];
    }

    fprintf(stderr, "ERROR: Tried to peak behind the buffer !");
    exit(1);
}

Token ScanValue(char *buffer, i32 *position, i32 size) {
    Token result = {0};
    result.start = *position;
    
    while(*position < size - 1 && isdigit(Peek(buffer, position, size))) {
        *position += 1;
    }

    result.end = *position;
    
    result.length = (result.end - result.start) + 1;
    result.repr.buffer = calloc(result.length, sizeof(char));
    char *startOfToken = buffer + result.start;
    memcpy(result.repr.buffer, startOfToken, result.length);

    result.tt = T_VALUE;
    return result;
}

Token ScanOperator(char *buffer, i32 position, i32 size) {
    Token result = {0};
    result.start = position;
    result.end = position;
    switch(buffer[position]) {
        case '.':
            result.tt = T_DOT;
            result.repr.symbol = '.';
        break;
        case '+':
            result.tt = T_PLUS;
            result.repr.symbol = '+';
        break;
        case '-':
            result.tt = T_MINUS;
            result.repr.symbol = '-';
        break;
        case '*':
            result.tt = T_MULTIPLY;
            result.repr.symbol = '*';
        break;
        case '/':
            result.tt = T_DIVIDE;
            result.repr.symbol = '/';
        break;
        case '=':
            result.tt = T_EQUAL;
            result.repr.symbol = '=';
        break;
        case '<':
            result.tt = T_LESSTHAN;
            result.repr.symbol = '<';
        break;
        case '>':
            result.tt = T_GREATERTHAN;
            result.repr.symbol = '>';
        break;
        default:
            fprintf(stderr, "TODO: We tried to parse a symbol which isn't in the language yet...\n");
            exit(1);
        break;
    }
    
    return result;
}

int main(int argc, char **argv) {
    if(argc < 2) {
        fprintf(stderr, "USAGE: kforth [path]\n");
        exit(1);
    }
    const char *path = argv[1];
    struct stat st;
    stat(path, &st);

    FILE *inputFile = fopen(path, "r");
    //TODO: This is a temporary buffer and should be replaced by a dynamically allocated one !
    char tempBuffer[1024] = {0};

    if(st.st_size >= 1024) {
        fprintf(stderr, "TODO: File size too big for tempBuffer replace tempbuffer with dynamic memory !");
        fclose(inputFile);
        exit(1);
    }

    i32 readBytes = fread(&tempBuffer, 1, st.st_size, inputFile);
    
    if(readBytes < st.st_size) {
        fprintf(stderr, "ERROR: Could not read the whole filestream");
        fclose(inputFile);
        exit(1);
    }

    //printf("%s\n", tempBuffer);

    i32 position = 0;
    Token tempToken[512] = {0};
    i32 nextFreeToken;
    while(position < st.st_size) 
    {

        if(isdigit(tempBuffer[position])) {
            tempToken[nextFreeToken] = ScanValue(tempBuffer, &position, st.st_size);
            position++;
            nextFreeToken++;
            continue;
        }

        if(ispunct(tempBuffer[position])) {
            tempToken[nextFreeToken] = ScanOperator(tempBuffer, position, st.st_size);
            position++;
            nextFreeToken++;
            continue;
        }
        
        if(isspace(tempBuffer[position])) {
            position++;
        }
    }

    Stack stack = {0};
    Command program[512] = {0};

    i32 nextFreeInstruction = 0;
    for(i32 i = 0; i < nextFreeToken; i++) {
        if(tempToken[i].tt == T_VALUE) {
            //Push these to the stack
            program[nextFreeInstruction].operation = PUSH;
            program[nextFreeInstruction].value = atof(tempToken[i].repr.buffer);
        }

        if(tempToken[i].tt == T_PLUS) {
            program[nextFreeInstruction].operation = PLUS;
        }

        if(tempToken[i].tt == T_DOT) {
            program[nextFreeInstruction].operation = OUTPUT;
        }

        if(tempToken[i].tt == T_MULTIPLY) {
            program[nextFreeInstruction].operation = MULTIPLY;
        }

        if(tempToken[i].tt == T_LESSTHAN) {
            program[nextFreeInstruction].operation = LESSTHAN;
        }

        if(tempToken[i].tt == T_GREATERTHAN) {
            program[nextFreeInstruction].operation = GREATERTHAN;
        }

        if(tempToken[i].tt == T_EQUAL) {
            program[nextFreeInstruction].operation = EQUAL;
        }

        nextFreeInstruction++;
    }

    for(i32 i = 0; i < nextFreeInstruction; i++) {
        HandleOperation(&program[i], &stack);
    }
}