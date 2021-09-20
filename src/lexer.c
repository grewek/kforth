#include "lexer.h"
#include "token_list.h"
#include "token.h"
#include "base_types.h"

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

/*TokenType PeekNextLetter(Token *tokenList, i32 tokenListSize,  i32 currentPosition) {
    i32 nextTokenPos = currentPosition + 1;

    if(nextTokenPos < tokenListSize) {
        return tokenList[nextTokenPos].tt;
    } else {
        fprintf(stderr, "ERROR: Out of Tokens peeking not possible !");
        exit(1);
    }
}*/

char *TokenString(char *buffer, u64 start, u64 length) {
    char *result = calloc(length + 1, sizeof(char));

    if(result == NULL) {
        fprintf(stderr, "ERROR: Could not allocate memory for Token String\n");
        exit(1);
    }
    char *startOfToken = buffer + start;
    memcpy(result, startOfToken, length);

    return result;
}

char PeekNextLetter(const char *buffer, u64 *currentPosition, u64 size) {
    if(*currentPosition < size) {
        return buffer[*currentPosition + 1];
    }

    fprintf(stderr, "ERROR: Tried to peak behind the buffer !");
    exit(1);
}

Token ScanValue(char *buffer, u64 *position, u64 size) {
    Token result = {0};
    result.start = *position;
    
    while(*position < size - 1 && isdigit(PeekNextLetter(buffer, position, size))) {
        *position += 1;
    }

    result.end = *position;
    result.length = ((result.end + 1) - result.start);
    result.repr.buffer = TokenString(buffer, result.start, result.length);
    result.tt = T_VALUE;
    return result;
}

Token ScanWord(char *buffer, u64 *position, u64 size) {
    Token result = {0};
    result.start  = *position;

    while(*position < size - 1 && (islower(PeekNextLetter(buffer, position, size)) || isupper(PeekNextLetter(buffer,position, size)))) {
        *position += 1;
    }

    result.end = *position;
    result.length = ((result.end + 1) - result.start);
    result.repr.buffer = TokenString(buffer, result.start, result.length);

    if(strcmp(result.repr.buffer, "swap") == 0) {
        result.tt = T_SWAP;
    } else if(strcmp(result.repr.buffer, "dup") == 0) {
        result.tt = T_DUP;
    } else {
        result.tt = T_WORD;
    }
    
    return result;
}

Token ScanOperator(char *buffer, u64 position) {
    Token result = {0};
    result.start = position;
    result.end = position;
    result.length = 1;
    switch(buffer[position]) {
        case ':':
            result.tt = T_COLON;
            result.repr.symbol = ':';
        break;
        case ';':
            result.tt = T_SEMICOLON;
            result.repr.symbol = ';';
        break;
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


TokenList GenerateTokenList(char *buffer, u64 size)
{
    u64 position = 0;
    TokenList tokens = InitializeTokenList();
    while(position < size) 
    {
        char currentChar = buffer[position];
        if(islower(currentChar)) {
            //TODO: We need to parse a word here...
            PushToken(&tokens, ScanWord(buffer, &position, size));
            position++;
            continue;
        }

        if(isdigit(buffer[position])) {
            PushToken(&tokens, ScanValue(buffer, &position, size));
            position++;
            continue;
        }

        if(ispunct(buffer[position])) {
            PushToken(&tokens, ScanOperator(buffer, position));
            position++;
            continue;
        }
        
        if(isspace(buffer[position])) {
            position++;
        }
    }

    return tokens;
}