#include "forth_parser.h"

void Parse(Parser parser) 
{
    while(!TokenStackEmpty(parser.tokens)) 
    {
        Token *currentToken = PeekToken(parser.tokens);

        switch(currentToken->tt)
        {
           case T_COLON:
                ParseFunctionDefinition(parser);
            break;
           default:
                ReportParserError(ERR_PARSER_OUTSIDE_FUNCTION);
                exit(1);
            break;

        }
    }
}

void ParseValue(Parser parser)
{
    Token valueToken = PopToken(parser.tokens);
    if(valueToken.tt != T_VALUE)
    {
        ReportParserError(ERR_PARSER_INVALID_VALUE);
    }
    u32 value = atoi(valueToken.repr.buffer);

    InsertPushOpcode(parser.gen, value);
}

void ParseOperatorPlus(Parser parser)
{
    PopToken(parser.tokens);
    InsertAddOpcode(parser.gen);
}

void ParseOperatorMinus(Parser parser)
{
    PopToken(parser.tokens);
    InsertSubOpcode(parser.gen);
}

void ParseOperatorMultiply(Parser parser)
{
    PopToken(parser.tokens);
    InsertMulOpcode(parser.gen);
}

void ParseOperatorDivide(Parser parser)
{
    PopToken(parser.tokens);
    InsertDivOpcode(parser.gen);
}

void ParseOperatorEqual(Parser parser)
{
    PopToken(parser.tokens);
    InsertEqOpcode(parser.gen);
}

void ParseOperatorLessThan(Parser parser)
{
    PopToken(parser.tokens);
    InsertLtOpcode(parser.gen);
}

void ParseOperatorGreaterThan(Parser parser)
{
    PopToken(parser.tokens);
    InsertGtOpcode(parser.gen);
}

void ParseIntrinsicOutput(Parser parser)
{
    PopToken(parser.tokens);
    InsertDotOpcode(parser.gen);
}

void ParseIntrinsicSwap(Parser parser)
{
    PopToken(parser.tokens);
    InsertSwapOpcode(parser.gen);
}
void ParseIntrinsicDup(Parser parser)
{
    PopToken(parser.tokens);
    InsertDupOpcode(parser.gen);
}

void ParseIntrinsicRotate(Parser parser)
{
    PopToken(parser.tokens);
    InsertRotateOpcode(parser.gen);
}

Token *ParseTerminals(Parser parser)
{
    while(!TokenStackEmpty(parser.tokens))
    {
        Token *currentToken = PeekToken(parser.tokens);

        switch(currentToken->tt)
        {
            case T_VALUE:
                ParseValue(parser);
            break;
            case T_PLUS:
                ParseOperatorPlus(parser);
            break;
            case T_MINUS:
                ParseOperatorMinus(parser);
            break;
            case T_DOT:
                ParseIntrinsicOutput(parser);
            break;
            case T_MULTIPLY:
                ParseOperatorMultiply(parser);
            break;
            case T_DIVIDE:
                ParseOperatorDivide(parser);
            break;
            case T_EQUAL:
                ParseOperatorEqual(parser);
            break;
            case T_LESSTHAN:
                ParseOperatorLessThan(parser);
            break;
            case T_GREATERTHAN:
                ParseOperatorGreaterThan(parser);
            break;
            case T_SWAP:
                ParseIntrinsicSwap(parser);
            break;
            case T_DUP:
                ParseIntrinsicDup(parser);
            break;
            case T_WORD:
                ParseCall(parser);
            break;
            case T_ROTATE:
                ParseIntrinsicRotate(parser);
            break;
            case T_STRING:
                fprintf(stderr, "TODO: Handle strings !");
                exit(1);
            break;
            default:
                //The terminal parser does not handle these cases
                //so we should return to the previous parser.
                return currentToken;
            break;
        }
    }

    return NULL;
}
void ParseFunctionBody(Parser parser)
{
    while(!TokenStackEmpty(parser.tokens))
    {
        Token *currentToken = PeekToken(parser.tokens);
        currentToken = ParseTerminals(parser);

        if(currentToken != NULL)
        {
            switch(currentToken->tt)
            {
                case T_COLON:
                    ReportParserError(ERR_PARSER_FUNC_DEF_INSIDE_FUNC);
                    exit(1);
                break;
                case T_IF:
                    ParseIf(parser);
                break;
                case T_SEMICOLON:
                    return;
                break;
                default:
                break;
            }
        }
    }
}

void ParseFunctionDefinition(Parser parser)
{
    //Pop the ':' symbol
    PopToken(parser.tokens);
    //Pop the name of the function
    //TODO: We need to generate somekind of lable here...
    //TODO: Solidify the concept here into a real hashmap.
    Token nameToken = PopToken(parser.tokens);
    u64 functionKey = _GenerateHashValue(nameToken.repr.buffer, 
            nameToken.length);

    //NOTE: Calculate the Entrypoint here ?
    u32 functionStart = parser.gen->instructionCount;
    parser.addressBook[functionKey % 1024] = functionStart;
    //Parse the whole function
    ParseFunctionBody(parser);
    //Parse the end of the function.
    ParseFunctionEnd(parser);
}

void ParseFunctionEnd(Parser parser)
{
    if(PeekToken(parser.tokens)->tt != T_SEMICOLON)
    {
        ReportParserError(ERR_PARSER_FUNCTION_WITHOUT_END);
        exit(1);
    }

    PopToken(parser.tokens);
    InsertRetOpcode(parser.gen);
}

void ParseCall(Parser parser)
{
    Token nameToken = PopToken(parser.tokens);
    u64 addressKey = _GenerateHashValue(nameToken.repr.buffer, 
            nameToken.length);

    u64 callAddress = parser.addressBook[addressKey % 1024];

    //TODO: Make sure that our "addressbook" contains those functions
    //      we try to call...
    InsertCallOpcode(parser.gen, callAddress);
}

void ParseIf(Parser parser)
{
    u64 ifTrueBranchEnd = 0;
    while(!TokenStackEmpty(parser.tokens))
    {
        Token *currentToken = PeekToken(parser.tokens);

        switch(currentToken->tt)
        {
            case T_IF:
            {
                //Pop off the if keyword we don't need it any longer.
                PopToken(parser.tokens);

                //Generate a if goto operation and store the position so we can
                //set the correct offset later.
                u64 elseJump = parser.gen->instructionCount;
                InsertIfGotoOpcode(parser.gen, 0); 

                //Parse the if block.
                ParseIfBody(parser);
                
                //Calculate the blocksize of the if branch
                u64 ifBlockEnd = parser.gen->instructionCount;
                u64 ifBlockSize = ifBlockEnd - elseJump;

                //Put the jump offset into the if goto opcode.
                parser.gen->memory[elseJump + 1] = ifBlockSize;

                //Store the position of the jump to the end of the if statement.
                //this gets filled out as soon as we hit the then keyword
                ifTrueBranchEnd = parser.gen->instructionCount;
                InsertGotoOpcode(parser.gen, 0);
            } break;

            case T_ELSE: 
            {
                PopToken(parser.tokens);
                ParseIfBody(parser);
            } break;

            case T_THEN: {
                u64 ifEnd = parser.gen->instructionCount;
                u64 elseBlockSize = ifEnd - ifTrueBranchEnd;
                parser.gen->memory[ifTrueBranchEnd + 1] = elseBlockSize;
                PopToken(parser.tokens);
                return;
            } break;

            default:
            break;
        }
    }

    ReportParserError(ERR_PARSER_IF_WITHOUT_THEN);
    exit(1);
}

void ParseIfBody(Parser parser)
{
    while(!TokenStackEmpty(parser.tokens))
    {
        Token *currentToken = PeekToken(parser.tokens);
        currentToken = ParseTerminals(parser);

        if(currentToken != NULL)
        {
            switch(currentToken->tt)
            {
            case T_COLON:
                ReportParserError(ERR_PARSER_FUNC_DEF_INSIDE_IF);
                exit(1);
            break;
            case T_SEMICOLON:
                ReportParserError(ERR_PARSER_FUNC_END_INSIDE_IF);
                exit(1);
                return;
            break;
            case T_IF:
                ParseIf(parser);
            break;
            default:
                return;
            break;
            }
        }
    }
}

u64 GetFunctionAddress(Parser parser, char *name, u64 size)
{
    u64 addressKey = _GenerateHashValue(name, size);
    u64 result = parser.addressBook[addressKey % 1024];
    return result;
}

void ReportParserError(const char *message)
{
    fprintf(stderr, "ERROR: %s\n", message);
}
