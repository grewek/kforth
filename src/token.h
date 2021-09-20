#ifndef _TOKEN_H_
#define _TOKEN_H_
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
} TokenType;

typedef struct  {
    TokenType tt;

    i32 start;
    i32 end;
    i32 length;

    union {
        char *buffer;
        char symbol;
    } repr;
    

} Token;

#endif