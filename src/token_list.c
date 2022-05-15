#include "token_list.h"

TokenList InitializeTokenList()
{
    TokenList result = {0};

    result._elementCount = 0;
    result._head = 0;

#ifdef LINUX_BUILD
    result._pageSize = sysconf(_SC_PAGE_SIZE);
#endif

#ifdef WINDOWS_BUILD
    SYSTEM_INFO info = {0};
    GetSystemInfo(&info);
    result._pageSize = info.dwPageSize;
#endif
    result._pageCount = 1;
    result._maxElementCount = (result._pageSize * result._pageCount) / sizeof(Token);

    result._tokens = calloc(result._maxElementCount, sizeof(Token));

    if (result._tokens == NULL)
    {
        fprintf(stderr, "ERROR: Initilization of the TokenList failed...\n");
        exit(1);
    }

    return result;
}

void FreeTokenList(TokenList *tokenList)
{
    for (u64 i = 0; i < tokenList->_elementCount; i++)
    {
        Token *currentToken = &tokenList->_tokens[i];

        // NOTE: We need a better way to handle freeing keywords or we have to manually add a case for
        //       every keyword which doesn't sound fun.
        switch (currentToken->tt)
        {
        case T_WORD:
        case T_VALUE:
        case T_SWAP:
        case T_DUP:
        case T_STRING:
            free(currentToken->repr.buffer);
            currentToken->repr.buffer = NULL;
            break;
        // The default case has no memory stored internally and can just be ignored.
        default:
            break;
        }
    }
    free(tokenList->_tokens);
}

void _ResizeTokenList(TokenList *tokenList)
{
    // TODO: Make sure that we actually have a valid list here !
    tokenList->_pageCount *= 2;
    tokenList->_maxElementCount = (tokenList->_pageSize * tokenList->_pageCount) / sizeof(Token);

    Token *newList = realloc(tokenList->_tokens, tokenList->_maxElementCount * sizeof(Token));
    tokenList->_tokens = newList;

    if (tokenList->_tokens == NULL)
    {
        fprintf(stderr, "ERROR: Resizing the TokenList failed...\n");
        exit(1);
    }
}

void PushToken(TokenList *tokenList, Token token)
{
    if (tokenList->_elementCount >= tokenList->_maxElementCount)
    {
        _ResizeTokenList(tokenList);
    }

    tokenList->_tokens[tokenList->_elementCount] = token;
    tokenList->_elementCount += 1;
}

Token PopToken(TokenList *tokenList)
{
    if (tokenList->_head < tokenList->_elementCount)
    {
        Token topToken = tokenList->_tokens[tokenList->_head];
        tokenList->_head += 1;
        return topToken;
    }

    fprintf(stderr, "ERROR: Out of tokens !\n");
    exit(1);
}

Token *PeekToken(TokenList *tokenList)
{
    if (tokenList->_head < tokenList->_elementCount)
    {
        Token *result = &tokenList->_tokens[tokenList->_head];
        return result;
    }

    fprintf(stderr, "ERROR: Out of tokens to peek into !\n");
    exit(1);
}

bool TokenStackEmpty(TokenList *tokenList)
{
    if (tokenList->_head >= tokenList->_elementCount)
    {
        return true;
    }

    return false;
}