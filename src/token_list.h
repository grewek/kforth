#ifndef _TOKEN_LIST_H_
#define _TOKEN_LIST_H_

#include "base_types.h"
#include "token.h"
typedef struct TokenList {
    u32 size;
    u32 head;
    u32 maxSize;

    Token *tokens;

} TokenList;

TokenList InitializeTokenList();
void FreeTokenList(TokenList *tokenList);

//NOTE: Functions starting with an underscore are only for internal usage...
void _ResizeTokenList(TokenList *tokenList);
void PushToken(TokenList *tokenList, Token token);

Token PopToken(TokenList *tokenList);
void PeekToken(TokenList *tokenList);

#endif