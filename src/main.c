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

//TODO: Seperate into different files for a better overview...

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
    SWAP,
    DUP,
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

    //TODO: Find out if there are any extensions for c that checks whether or not we are covering all the things in the
    //      enumeration...
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

        case DUP:
        UnaryReferenceOperation(stack, DuplicateValue);
    }
}

typedef enum {
    T_VALUE,
    T_PLUS,
    T_MINUS,
    T_DOT,
    T_MULTIPLY,
    T_DIVIDE,
    T_EQUAL,
    T_LESSTHAN,
    T_GREATERTHAN,
    T_SWAP,
    T_DUP,
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

char *TokenString(char *buffer, i32 start, i32 length) {
    char *result = calloc(length, sizeof(char));
    char *startOfToken = buffer + start;
    memcpy(result, startOfToken, length);

    return result;
}

Token ScanValue(char *buffer, i32 *position, i32 size) {
    Token result = {0};
    result.start = *position;
    
    while(*position < size - 1 && isdigit(Peek(buffer, position, size))) {
        *position += 1;
    }

    result.end = *position;
    result.length = ((result.end + 1) - result.start);
    result.repr.buffer = TokenString(buffer, result.start, result.length);
    result.tt = T_VALUE;
    return result;
}

Token ScanWord(char *buffer, i32 *position, i32 size) {
    Token result = {0};
    result.start  = *position;

    while(*position < size - 1 && (islower(Peek(buffer, position, size)) || isupper(Peek(buffer,position, size)))) {
        *position += 1;
    }

    result.end = *position;
    result.length = ((result.end + 1) - result.start);
    result.repr.buffer = TokenString(buffer, result.start, result.length);

    if(strcmp(result.repr.buffer, "swap") == 0) {
        result.tt = T_SWAP;
    }

    if(strcmp(result.repr.buffer, "dup") == 0) {
        result.tt = T_DUP;
    }
    
    return result;
}

Token ScanOperator(char *buffer, i32 position) {
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
        //TODO: The lexer needs to recognize symbols that are longer than one symbol
        //TODO: There are words in forth as well so we need to handle all the letters
        //TODO: Refactor this process out of the main function
        //TODO: We could advance position and nextFreeToken at the end of the loop instead of doing in every branch...
        //TODO: The token list is on the stack right now and has only space for 512 tokens this will probably not suffice
        //      for a real forth program so we need to make this dynamic sooner or later
        char currentChar = tempBuffer[position];
        if(islower(currentChar)) {
            //TODO: We need to parse a word here...
            tempToken[nextFreeToken] = ScanWord(tempBuffer, &position, st.st_size);
            position++;
            nextFreeToken++;
            continue;
        }

        if(isdigit(tempBuffer[position])) {
            tempToken[nextFreeToken] = ScanValue(tempBuffer, &position, st.st_size);
            position++;
            nextFreeToken++;
            continue;
        }

        if(ispunct(tempBuffer[position])) {
            tempToken[nextFreeToken] = ScanOperator(tempBuffer, position);
            position++;
            nextFreeToken++;
            continue;
        }
        
        if(isspace(tempBuffer[position])) {
            position++;
        }
    }

    //Parsing the lexed application into executable forth code
    Command program[512] = {0};
    i32 nextFreeInstruction = 0;
    for(i32 i = 0; i < nextFreeToken; i++) {
        
        Token *currentToken = &tempToken[i];
        switch(currentToken->tt) {
            case T_VALUE:
                program[nextFreeInstruction].operation = PUSH;
                program[nextFreeInstruction].value = atof(currentToken->repr.buffer);
            break;
            
            case T_PLUS:
                program[nextFreeInstruction].operation = PLUS;
            break;
            
            case T_MINUS:
                program[nextFreeInstruction].operation = MINUS;
            break;

            case T_DOT:
                program[nextFreeInstruction].operation = OUTPUT;
            break;
            
            case T_MULTIPLY:
                program[nextFreeInstruction].operation = MULTIPLY;
            break;
            
            case T_DIVIDE:
                program[nextFreeInstruction].operation = DIVIDE;
            break;
            
            case T_LESSTHAN:
                program[nextFreeInstruction].operation = LESSTHAN;
            break;
            
            case T_GREATERTHAN:
                program[nextFreeInstruction].operation = GREATERTHAN;
            break;
            
            case T_EQUAL:
                program[nextFreeInstruction].operation = EQUAL;
            break;
            
            case T_SWAP:
                program[nextFreeInstruction].operation = SWAP;
            break;

            case T_DUP:
                program[nextFreeInstruction].operation = DUP;
            break;
        }

        nextFreeInstruction++;

    }

    //Execution of the parsed application...
    Stack stack = {0};
    for(i32 i = 0; i < nextFreeInstruction; i++) {
        HandleOperation(&program[i], &stack);
    }
}