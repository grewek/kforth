#ifndef _TOKEN_LIST_H_
#define _TOKEN_LIST_H_

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#include "base_types.h"
#include "token.h"

/*
@Warn: All elements marked with an underscore are for internal usage only and shouldn't be touched by the outside world.
@Description: A Dynamically growing list of tokens.

@_elementCount: The count of elements that the list currently contains. At initilization this value is 0.
@_head: The element that is currently the next element that get's popped off. Will increase after every use of PopToken()
@_maxElementCount: The maximum amount of elements the list can currently hold. The lists needs to be resized if this value is exceeded.
@_pageSize: A size of a memory page from the operating system.
@_pageCount: Amount of memory pages the TokenList has currently in use. At initilization this value is 1.
@_tokens: The backing memory area where all tokens are stored.
*/
typedef struct TokenList {
    u64 _elementCount;
    u64 _head;
    u64 _maxElementCount;
    u64 _pageSize;
    u64 _pageCount;

    Token *_tokens;

} TokenList;

/*
@Description: Reserves memory and initializes a new Token List.
@Returns: A Empty Token List ready to use
*/
TokenList InitializeTokenList();
/*
@Description: Returns the Memory used by the Token List, and all of it's currently contained Tokens, 
              to the Operating system.

@tokenList:   The Token List which memory will be released
*/
void FreeTokenList(TokenList *tokenList);

/*
@Warn:        This Function is for _internal_ usage only use at your own risk.
@Description: Doubles the size of the given token list.

@tokenList:   The tokenlist pointer which size needs to be increased.
*/
void _ResizeTokenList(TokenList *tokenList);

/*
@Description: Pushes a token onto the end of the given token list. If the token list is out
              of memory it automatically calls _ResizeTokenList which increases the amount
              of elements the token list can hold.

@tokenList:   The token list where the new token shall be pushed into.
@token:       The token which needs to be pushed.
*/
void PushToken(TokenList *tokenList, Token token);

/*
@Description: Retrieves a shallow copy of the next token, on the front of the list.

@tokenList:   The tokenList from which the token needs to be taken off.

@Returns:     A shallow copy of a token struct.
*/
Token PopToken(TokenList *tokenList);

/*
@Description: Returns a pointer to the next token in the token list but does not remove it.

@tokenList:   The tokenlist in which you want to peek into.

@Returns:     A pointer to the current first token in the list.
*/
Token *PeekToken(TokenList *tokenList);

/*
@Description: Checks if there are still elements left in the list to pop or peek,

@tokenList:   The list which needs to be checked for elements left.

@Returns: True if the list had no more elements to retrieve or False if there are still elements left.
*/
bool TokenStackEmpty(TokenList *tokenList);

#endif