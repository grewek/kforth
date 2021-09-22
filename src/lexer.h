#ifndef _LEXER_H_
#define _LEXER_H_

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#include "base_types.h"
#include "token_list.h"

typedef struct Lexer {
    char *source;
    u64 position;
    u64 size;
} Lexer;

Lexer InitializeLexer(const char *sourceFilePath);
TokenList GenerateTokenList(const char *sourcePath);

void ConsumeCharacter(Lexer *lexer);
char PeekCharacter(Lexer *lexer);

#endif