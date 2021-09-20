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
    if(lexer->position < lexer->size) {
        return lexer->source[lexer->position + 1];
    }

    fprintf(stderr, "ERROR: Tried to peak behind the buffer !");
    exit(1);
}

void ConsumeCharacter(Lexer *lexer) {
    if(lexer->position >= lexer->size) {
        fprintf(stderr, "ERROR: Out of Characters to consume !");
        exit(1);
    }

    lexer->position += 1;
}

Token ScanValue(Lexer *lexer) {
    Token result = {0};
    result.start = lexer->position;
    
    while(lexer->position < (lexer->size - 1) && isdigit(PeekCharacter(lexer))) {
        ConsumeCharacter(lexer);
    }

    result.end = lexer->position;
    result.length = ((result.end + 1) - result.start);
    result.repr.buffer = TokenString(lexer->source, result.start, result.length);
    result.tt = T_VALUE;
    return result;
}

Token ScanWord(Lexer *lexer) {
    Token result = {0};
    result.start  = lexer->position;

    while(lexer->position < (lexer->size - 1) && (islower(PeekCharacter(lexer)) || isupper(PeekCharacter(lexer)))) {
        ConsumeCharacter(lexer);
    }

    result.end = lexer->position;
    result.length = ((result.end + 1) - result.start);
    result.repr.buffer = TokenString(lexer->source, result.start, result.length);

    if(strcmp(result.repr.buffer, "swap") == 0) {
        result.tt = T_SWAP;
    } else if(strcmp(result.repr.buffer, "dup") == 0) {
        result.tt = T_DUP;
    } else {
        result.tt = T_WORD;
    }
    
    return result;
}

Token ScanOperator(Lexer *lexer) {
    Token result = {0};
    result.start = lexer->position;
    result.end = lexer->position;
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
    
    return result;
}


TokenList GenerateTokenList(const char *sourcePath)
{
    Lexer lexer = InitializeLexer(sourcePath);
    TokenList tokens = InitializeTokenList();
    while(lexer.position < lexer.size)
    {
        char currentChar = lexer.source[lexer.position];
        if(islower(currentChar)) {
            PushToken(&tokens, ScanWord(&lexer));
            ConsumeCharacter(&lexer);
            continue;
        }

        if(isdigit(currentChar)) {
            PushToken(&tokens, ScanValue(&lexer));
            ConsumeCharacter(&lexer);
            continue;
        }

        if(ispunct(currentChar)) {
            PushToken(&tokens, ScanOperator(&lexer));
            ConsumeCharacter(&lexer);
            continue;
        }
        
        if(isspace(currentChar)) {
            ConsumeCharacter(&lexer);
        }
    }

    //NOTE: We free the source buffer here...
    free(lexer.source);
    return tokens;
}