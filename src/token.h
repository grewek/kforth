#ifndef _TOKEN_H_
#define _TOKEN_H_

#include "base_types.h"

typedef enum {
    T_VALUE,
    T_PLUS,
    T_MINUS,
    T_DOT,
    T_MULTIPLY,
    T_DIVIDE,
    T_EQUAL,
    T_LESSTHAN,
    T_GREATERTHAN,
    T_SWAP,
    T_DUP,
    T_COLON,
    T_SEMICOLON,
    T_WORD,
    T_STRING,
    T_ROTATE,
    T_IF,
    T_THEN,
    T_ELSE,
} TokenType;

typedef struct {
    TokenType tt;

    u64 start;
    u64 length;

    union {
        char *buffer;
        char symbol;
    } repr;
    

} Token;

#endif