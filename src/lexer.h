#ifndef _LEXER_H_
#define _LEXER_H_

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#ifdef LINUX_BUILD
    #include <sys/types.h>
    #include <sys/stat.h>
#endif
#include <string.h>

#include "base_types.h"
#include "token.h"
#include "token_list.h"

typedef struct Lexer {
    const char *fileName;
    char *source;
    u64 line;
    u64 rawPosition;
    u64 linePosition;
    u64 size;
} Lexer;

Lexer InitializeLexer(const char *sourceFilePath);
TokenList GenerateTokenList(const char *sourcePath);

char ConsumeCharacter(Lexer *lexer);
char PeekCharacter(Lexer *lexer);
bool MatchAndConsume(Lexer *lexer, char symbol);

Token ScanValue(Lexer *lexer);
Token ScanWord(Lexer *lexer);
Token ScanString(Lexer *lexer);
Token ScanOperator(Lexer *lexer);

void ConsumeComment(Lexer *lexer);
void WordToKeyword(Token *token);


void ReportError(Lexer *lexer, const char *errorMessage);
#endif