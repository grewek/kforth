#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "token_list.h"

TokenList InitializeTokenList() {
    TokenList result = {0};

    result.size = 0;
    result.head = 0;
    result.maxSize = 64;
    result.tokens = calloc(result.maxSize, sizeof(Token));

    if(result.tokens == NULL) {
        fprintf(stderr, "ERROR: Initilization of the TokenList failed...\n");
        exit(1);
    }

    return result;
}

void FreeTokenList(TokenList *tokenList) {
    for(u64 i = 0; i < tokenList->size; i++) {
        Token *currentToken = &tokenList->tokens[i];

        switch(currentToken->tt) {
            case T_WORD:
            case T_VALUE:
                fprintf(stderr, "INFO: Freeing Memory of element: %s\n", currentToken->repr.buffer);
                free(currentToken->repr.buffer);
                currentToken->repr.buffer = NULL;
            break;
            //The default case has no memory stored internally and can just be freed...
            default: break;
        }
    }
    free(tokenList->tokens);
}

void _ResizeTokenList(TokenList *tokenList) {
    tokenList->maxSize *= 2;
    tokenList->tokens = realloc(tokenList->tokens, tokenList->maxSize);

    if(tokenList->tokens == NULL) {
        fprintf(stderr, "ERROR: Resizing the TokenList failed...\n");
        exit(1);
    }
}

void PushToken(TokenList *tokenList, Token token) {
    if(tokenList->size >= tokenList->maxSize) {
        _ResizeTokenList(tokenList);
    }
    //TODO: Find out what works better assignment or memcpy ?
    //Token *tokenSlot = tokenList->tokens + tokenList->size;
    //memcpy(tokenSlot, &token, sizeof(Token));
    tokenList->tokens[tokenList->size] = token;
    tokenList->size += 1;
}

Token PopToken(TokenList *tokenList) {
    Token topToken = tokenList->tokens[tokenList->head];
    tokenList->head += 1;
    
    return topToken;
}


/*void PeekToken(TokenList *tokenList) {
    tokenList = NULL;
}*/