#ifndef _FORTH_PARSER_H_
#define _FORTH_PARSER_H_
#include "token_list.h"
#include "forth_vm_backend.h"
#include "hash_map.h"

//NOTE: Define all the error messages here
//TODO: Reword these error messages currently they are not very user friendly...
#define ERR_PARSER_INVALID_VALUE "Tried to push something on the stack that isnt a value"
#define ERR_PARSER_FUNCTION_WITHOUT_END "Did not find a valid ending for the current function did you forgot a ';' ?"
#define ERR_PARSER_FUNC_DEF_INSIDE_FUNC "You cannot define a function inside another function"
#define ERR_PARSER_FUNC_DEF_INSIDE_IF "Tried to parse a function inside an if statement"
#define ERR_PARSER_FUNC_END_INSIDE_IF "Tried to parse the end of a function definition inside an if statement"
#define ERR_PARSER_IF_WITHOUT_THEN "If statement has no end, use the 'then' keyword to mark the ending of the function"
#define ERR_PARSER_OUTSIDE_FUNCTION "Tried to parse a token outside a function definition"
typedef struct Parser {
    TokenList *tokens;
    u64 *addressBook;

    Generator *gen;
} Parser;

void ReportParserError(const char *message);
void Parse(Parser parser);
void ParseValue(Parser parser);

void ParseOperatorPlus(Parser parser);
void ParseOperatorMinus(Parser parser);
void ParseOperatorMultiply(Parser parser);
void ParseOperatorDivide(Parser parser);
void ParseOperatorEqual(Parser parser);
void ParseOperatorLessThan(Parser parser);
void ParseOperatorGreaterThan(Parser parser);

void ParseIntrinsicOutput(Parser parser);
void ParseIntrinsicSwap(Parser parser);
void ParseIntrinsicDup(Parser parser);
void ParseIntrinsicRotate(Parser parser);

void ParseCall(Parser parser);
void ParseFunctionDefinition(Parser parser);
void ParseFunctionBody(Parser parser);
void ParseFunctionEnd(Parser parser);

void ParseIf(Parser parser);
void ParseIfBody(Parser parser);

Token *ParseTerminals(Parser parser);
u64 GetFunctionAddress(Parser parser, char *name, u64 size);
#endif
