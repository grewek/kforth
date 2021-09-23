#include <stdio.h>

#include <assert.h>
#include <stdbool.h>
#include <ctype.h>

#include "base_types.h"
#include "hash_map.h"
#include "lexer.h"
#include "token_list.h"

#include "forth_stack.h"
#include "forth_function.h"
#include "forth_intrinsics.h"
#include "forth_evaluator.h"


void ParseForthProgram(ForthFunction *prog, HashMap *functions, TokenList *tokenList) {

    while(!TokenStackEmpty(tokenList)) {
        
        Token currentToken = PopToken(tokenList);

        assert(prog->count < prog->size);
        
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
    RunFunction(&main, &map, &stack);
    //EvaluateFunction(&main, &map, &stack);
    FreeHashMap(&map);
    FreeFunction(&main);
    FreeTokenList(&list);
}