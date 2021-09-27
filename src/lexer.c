#include "lexer.h"

Lexer InitializeLexer(const char *sourceFilePath) {
    Lexer result = {0};

    struct stat st;
    stat(sourceFilePath, &st);

    FILE *source = fopen(sourceFilePath, "r");
    result.size = st.st_size;
    result.source = calloc(st.st_size, sizeof(char));

    if(result.source == NULL) {
        fprintf(stderr, "ERROR: Could not allocate memory for the source file !");
        fclose(source);
        exit(1);
    }

    u64 readBytes = fread(result.source, 1, result.size, source);
    fclose(source);

    if(readBytes < result.size) {
        fprintf(stderr, "ERROR: Could not read the whole filestream");
        free(result.source);
        fclose(source);
        exit(1);
    }

    return result;
}

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

char PeekCharacter(Lexer *lexer) {
    if(lexer->position <= lexer->size) {
        return lexer->source[lexer->position + 1];
    }

    fprintf(stderr, "ERROR: Tried to peak behind the buffer !");
    exit(1);
}

void ConsumeCharacter(Lexer *lexer) {
    if(lexer->position > lexer->size) {
        fprintf(stderr, "ERROR: Out of Characters to consume !");
        exit(1);
    }

    lexer->position += 1;
}

Token ScanValue(Lexer *lexer) {
    Token result = {0};
    result.start = lexer->position;

    while(lexer->position < lexer->size && isdigit(PeekCharacter(lexer))) {
        ConsumeCharacter(lexer);
    }
    ConsumeCharacter(lexer);

    result.length = lexer->position - result.start;
    result.repr.buffer = TokenString(lexer->source, result.start, result.length);
    result.tt = T_VALUE;
    return result;
}

Token ScanWord(Lexer *lexer) {
    Token result = {0};
    result.start  = lexer->position;
    
    char nextCharacter = PeekCharacter(lexer);
    
    while(lexer->position < lexer->size && (islower(nextCharacter) || isupper(nextCharacter))) {
        ConsumeCharacter(lexer);
        nextCharacter = PeekCharacter(lexer);
    }
    ConsumeCharacter(lexer);

    result.length = lexer->position - result.start;
    result.repr.buffer = TokenString(lexer->source, result.start, result.length);
    result.tt = T_WORD;
    
    return result;
}

void ConsumeComment(Lexer *lexer) {
    while(lexer->position < lexer->size && lexer->source[lexer->position] != ')') {
        ConsumeCharacter(lexer);
    }
    ConsumeCharacter(lexer);
}

Token ScanString(Lexer *lexer) {
    Token result = {0};
    result.start = lexer->position;

    while(lexer->position < lexer->size && lexer->source[lexer->position] != '"') {
        ConsumeCharacter(lexer);
    }

    result.length = lexer->position - result.start;
    ConsumeCharacter(lexer);
    result.repr.buffer = TokenString(lexer->source, result.start, result.length);
    result.tt = T_STRING;

    return result;
}

Token ScanOperator(Lexer *lexer) {
    Token result = {0};
    result.start = lexer->position;
    //TODO: There are symbols with more than one character i.e. comments, we need to handle these as well
    result.length = 1;
    switch(lexer->source[lexer->position]) {
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
    
    ConsumeCharacter(lexer);
    return result;
}


void WordToKeyword(Token *token) {
    if(strcmp(token->repr.buffer, "swap") == 0) {
        token->tt = T_SWAP;
    } else if(strcmp(token->repr.buffer, "dup") == 0) {
        token->tt = T_DUP;
    }
}

TokenList GenerateTokenList(const char *sourcePath)
{
    Lexer lexer = InitializeLexer(sourcePath);
    TokenList tokens = InitializeTokenList();

    while(lexer.position < lexer.size)
    {
        Token currentToken;
        char currentChar = lexer.source[lexer.position];
        if(islower(currentChar)) {
            currentToken = ScanWord(&lexer);
            WordToKeyword(&currentToken);
        }
        else if(isdigit(currentChar)) {
            currentToken = ScanValue(&lexer);
        }
        else if(currentChar == '(') {
            ConsumeComment(&lexer);
            continue;
        }
        else if(ispunct(currentChar)) {
            if(currentChar == '.' && PeekCharacter(&lexer) == '"') {
                ConsumeCharacter(&lexer); // '.'
                ConsumeCharacter(&lexer); // '"'
                currentToken = ScanString(&lexer);
            } else {
                currentToken = ScanOperator(&lexer);
            }
        }
        else if(isspace(currentChar)) {
            ConsumeCharacter(&lexer);
            continue;
        }

        PushToken(&tokens, currentToken);
    }

    //NOTE: We free the source buffer here...
    free(lexer.source);
    return tokens;
}