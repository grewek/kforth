#ifndef _LEXER_H_
#define _LEXER_H_

#include "base_types.h"
#include "token_list.h"

TokenList GenerateTokenList(char *buffer, u64 size);

#endif