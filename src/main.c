#include <stdio.h>

#include <stdbool.h>
#include <ctype.h>

#include "base_types.h"
#include "hash_map.h"
#include "lexer.h"
#include "token_list.h"

#include "forth_function.h"
#include "forth_intrinsics.h"
#include "forth_stack.h"

void ParseForthProgram(ForthFunction *prog, HashMap *functions, TokenList *tokenList) {

    while(!TokenStackEmpty(tokenList)) {
        
        Token currentToken = PopToken(tokenList);
        switch(currentToken.tt) {
            case T_VALUE:
                prog->instructions[prog->count].operation = PUSH;
                prog->instructions[prog->count].valueType.value = atof(currentToken.repr.buffer);
            break;
            
            case T_PLUS:
                prog->instructions[prog->count].operation = PLUS;
            break;
            
            case T_MINUS:
                prog->instructions[prog->count].operation = MINUS;
            break;

            case T_DOT:
                prog->instructions[prog->count].operation = OUTPUT;
            break;
            
            case T_MULTIPLY:
                prog->instructions[prog->count].operation = MULTIPLY;
            break;
            
            case T_DIVIDE:
                prog->instructions[prog->count].operation = DIVIDE;
            break;
            
            case T_LESSTHAN:
                prog->instructions[prog->count].operation = LESSTHAN;
            break;
            
            case T_GREATERTHAN:
                prog->instructions[prog->count].operation = GREATERTHAN;
            break;
            
            case T_EQUAL:
                prog->instructions[prog->count].operation = EQUAL;
            break;
            
            case T_SWAP:
                prog->instructions[prog->count].operation = SWAP;
            break;

            case T_DUP:
                prog->instructions[prog->count].operation = DUP;
            break;

            case T_COLON:
                if(PeekToken(tokenList)->tt == T_WORD) {
                    Token name = PopToken(tokenList);
                    ForthFunction *emptyFunction = EmptyFunctionWithKey(functions, name.repr.buffer, name.length);
                    ParseForthProgram(emptyFunction, functions, tokenList);
                    continue;
                } else {
                    fprintf(stderr, "ERROR: Invalid function declaration !\n");
                    exit(1);
                }
            break;
            
            case T_SEMICOLON:
                return;
            break;

            case T_WORD:
                prog->instructions[prog->count].operation = WORD;
                prog->instructions[prog->count].valueType.name.name = currentToken.repr.buffer;
                prog->instructions[prog->count].valueType.name.size = currentToken.length;
                break;
        }

        prog->count += 1;
    }
}

void EvaluateFunction(ForthFunction *func, HashMap *map, Stack *stack) {
    for(u64 i = 0; i < func->count; i++) {
        //NOTE: If the user tries to call a function that he defined HandleOperation will give a pointer to the name
        //      structure back so we can search the function inside the function dictionary...
        struct Name *fname = HandleOperation(&func->instructions[i], stack);

        if(fname != NULL) {
            char *key = fname->name;
            u64 size = fname->size;

            ForthFunction *func = GetValue(map, key, size);
            EvaluateFunction(func, map, stack);
        }
    }
}

int main(int argc, char **argv) {
    if(argc < 2) {
        fprintf(stderr, "USAGE: kforth [path]\n");
        exit(1);
    }

    const char *path = argv[1];

    HashMap map = InitializeHashMap();

    ForthFunction main = InitializeFunction(128);
    TokenList list = GenerateTokenList(path);
    ParseForthProgram(&main, &map, &list);
    Stack stack = {0};

    //Clean evertyhing up so that valgrind doesn't complain for now...
    //TODO: Handle clean up in a case where there is a error we can't recover from !
    EvaluateFunction(&main, &map, &stack);
    FreeHashMap(&map);
    FreeFunction(&main);
    FreeTokenList(&list);
}