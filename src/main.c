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
#include "forth_evaluator.h"

void ParseExpression(ForthFunction *prog, TokenList *tokenList);

u32 FindPositionOfToken(TokenList *tokenList, TokenType searchedToken) {
    u32 position = 0;
    //TODO: This looks and feels hackish i should really figure out a better
    //      way to handle branches...
    //NOTE: Also we are using internal fields of the tokenlist here so we either
    //      move this into the tokenlist file or we find a better way to handle branches!
    for(u32 i = tokenList->_head; i < tokenList->_elementCount; i++) {
        
        if(tokenList->_tokens[i].tt == searchedToken)
            return position;

        switch(tokenList->_tokens[i].tt) {
            case T_IF:
                //NOTE: Add two instructions here as we need to push a Jump on Equal and a normal Jump on our instruction list.
                position += 2;
            break;
            case T_COLON:
            case T_SEMICOLON:
                //NOTE: These aren't really instruction they just ask the parser to start/end a function. So they don't get counted.
                continue;
            break;
            default:
                //NOTE: Everything else is right now one instruction long.
                position += 1;
            break;
        }
    }

    return -1;
}

void ParseStatement(ForthFunction *prog, HashMap *functions, TokenList *tokenList) {
    while(!TokenStackEmpty(tokenList)) {
        Token *currentToken = PeekToken(tokenList);

        assert(prog->count < prog->size);

        switch(currentToken->tt) {

            case T_IF: {
                u32 ifAddress = prog->count + 1;
                u32 elseAddress = prog->count + FindPositionOfToken(tokenList, T_ELSE);
                AddBranchInstructions(prog, ifAddress, elseAddress);
            } break;
            
            case T_THEN: {
                //TODO: Silently ignores multiple "then" keywords in a nested if statement ! Needs fixing.
            }
            break;
            
            case T_ELSE: {
                u32 thenAddress = prog->count + FindPositionOfToken(tokenList, T_THEN);
                AddJumpInstruction(prog, JMP, thenAddress);
            } break;

            case T_COLON:
                //TODO: Refactor this horrible sphaghetti code...
                PopToken(tokenList);
                if(PeekToken(tokenList)->tt == T_WORD) {
                    Token name = PopToken(tokenList);
                    ForthFunction *emptyFunction = EmptyFunctionWithKey(functions, name.repr.buffer, name.length);
                    ParseStatement(emptyFunction, functions, tokenList);
                    continue;
                } else {
                    fprintf(stderr, "ERROR: Invalid function declaration !\n");
                    exit(1);
                }
            break;
            
            case T_WORD:
                AddStringInstruction(prog, WORD, currentToken->repr.buffer, currentToken->length);
            break;
            
            case T_SEMICOLON:
                PopToken(tokenList);
                return;
            break;
            
            default:
                ParseExpression(prog, tokenList);
                continue;
            break;
        }

        PopToken(tokenList);
    }
}

void ParseExpression(ForthFunction *prog, TokenList *tokenList) {

    while(!TokenStackEmpty(tokenList)) {
        Token *currentToken = PeekToken(tokenList);

        assert(prog->count < prog->size);
        
        switch(currentToken->tt) {
            case T_VALUE:
                AddIntInstruction(prog, PUSH, atoi(currentToken->repr.buffer));
            break;
            
            case T_PLUS:
                AddVoidInstruction(prog, PLUS);
            break;
            
            case T_MINUS:
                AddVoidInstruction(prog, MINUS);
            break;

            case T_DOT:
                AddVoidInstruction(prog, OUTPUT);
            break;
            
            case T_MULTIPLY:
                AddVoidInstruction(prog, MULTIPLY);
            break;
            
            case T_DIVIDE:
                AddVoidInstruction(prog, DIVIDE);
            break;
            
            case T_LESSTHAN:
                AddVoidInstruction(prog, LESSTHAN);
            break;
            
            case T_GREATERTHAN:
                AddVoidInstruction(prog, GREATERTHAN);
            break;
            
            case T_EQUAL:
                AddVoidInstruction(prog, EQUAL);
            break;
            
            case T_ROTATE:
                AddVoidInstruction(prog, ROTATE);
            break;
            
            case T_SWAP:
                AddVoidInstruction(prog, SWAP);
            break;

            case T_DUP:
                AddVoidInstruction(prog, DUP);
            break;

            case T_COLON:
                return;
            break;
            
            case T_SEMICOLON:
                return;
            break;

            case T_STRING:
                AddStringInstruction(prog, DEF_STRING, currentToken->repr.buffer, currentToken->length);
            break;

            case T_WORD:
                AddStringInstruction(prog, WORD, currentToken->repr.buffer, currentToken->length);
            break;
            
            case T_IF:
                return;
            break;

            case T_THEN:
                return;
            break;

            case T_ELSE:
                return;
            break;
        }

        PopToken(tokenList);
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
    ParseStatement(&main, &map, &list);
    //ParseForthProgram(&main, &map, &list);
    Stack stack = {0};

    //Clean evertyhing up so that valgrind doesn't complain for now...
    //TODO: Handle clean up in a case where there is a error we can't recover from !
    RunFunction(&main, &map, &stack);
    //EvaluateFunction(&main, &map, &stack);
    FreeHashMap(&map);
    FreeFunction(&main);
    FreeTokenList(&list);
}