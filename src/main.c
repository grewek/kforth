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

void CalculateBranchSize(ForthFunction *prog, TokenList *tokenList) {
    //TODO: Should this only calculate the branch size and not set it ?
    u32 markerStart = prog->count - 1;
    PopToken(tokenList);
    ParseExpression(prog, tokenList);
    u32 markerEnd = prog->count;
    i32 branchSize = markerEnd - markerStart;
    
    InsertRelativePosition(prog, markerStart, branchSize);
}

void ParseIfStatement(ForthFunction *prog, TokenList *tokenList) {
    //TODO: Currently there is no support for nested if then statements.
    AddConditionalGoto(prog);
    CalculateBranchSize(prog, tokenList);

    if(PeekToken(tokenList)->tt == T_ELSE) {
        AddUnconditionalGoto(prog);
        CalculateBranchSize(prog, tokenList);
    }
    
}

void ParseStatement(ForthFunction *prog, HashMap *functions, TokenList *tokenList) {
    while(!TokenStackEmpty(tokenList)) {
        Token *currentToken = PeekToken(tokenList);

        assert(prog->count < prog->size);

        switch(currentToken->tt) {

            case T_IF: {
                ParseIfStatement(prog, tokenList);
            } break;
            
            case T_THEN: {
                //TODO: Get's handled in the T_IF case as it belongs to a if statement and cannot be parsed alone !
                //TODO: If we hit this case anyway a error was made by the user and we should report it !
                //      i.e. "found then without if" or something like that...
            }
            break;
            
            case T_ELSE: {
                //TODO: Get's handled in the T_IF case as it belongs to a if statement and cannot be parsed alone !
                //TODO: If we hit this case anyway a error was made by the user and we should report it !
                //      i.e. "found else without if" or something like that...
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