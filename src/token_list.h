#ifndef _TOKEN_LIST_H_
#define _TOKEN_LIST_H_

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "base_types.h"
#include "token.h"
//NOTE: Functions and fields that are starting with an underscore are only for internal usage...
typedef struct TokenList {
    u64 _elementCount;
    u64 _head;
    u64 _maxElementCount;
    u64 _pageSize;
    u64 _pageCount;

    Token *_tokens;

} TokenList;

TokenList InitializeTokenList();
void FreeTokenList(TokenList *tokenList);


void _ResizeTokenList(TokenList *tokenList);
void PushToken(TokenList *tokenList, Token token);

Token PopToken(TokenList *tokenList);
Token *PeekToken(TokenList *tokenList);
bool TokenStackEmpty(TokenList *tokenList);

#endif