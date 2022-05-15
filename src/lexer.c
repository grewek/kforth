#include "lexer.h"

void ReportError(Lexer *lexer, const char *errorMessage)
{
    fprintf(stderr, "(%s:%ld:%ld) Error: %s\n", lexer->fileName, lexer->line, lexer->linePosition, errorMessage);
    exit(EXIT_FAILURE);
}

#ifdef LINUX_BUILD
void LinuxReadWholeFile(const char *sourceFilePath, Lexer *result)
{
    struct stat st;
    stat(sourceFilePath, &st);

    FILE *source = fopen(sourceFilePath, "r");
    result->size = st.st_size;
    result->fileName = sourceFilePath;
    result->source = calloc(st.st_size, sizeof(char));

    if (result->source == NULL)
    {
        fprintf(stderr, "ERROR: Could not allocate memory for the source file !");
        fclose(source);
        exit(EXIT_FAILURE);
    }

    u64 readBytes = fread(result->source, 1, result->size, source);

    if (readBytes < result->size)
    {
        fprintf(stderr, "ERROR: Could not read the whole filestream");
        free(result->source);
        fclose(source);
        exit(EXIT_FAILURE);
    }

    fclose(source);
}
#endif

#ifdef WINDOWS_BUILD
void Win32ReadWholeFile(const char *sourceFilePath, Lexer *result)
{
    OFSTRUCT info = {0};
    HFILE fileHandle = OpenFile(sourceFilePath, &info, OF_READ);

    if (fileHandle == HFILE_ERROR)
    {
        fprintf(stderr, "ERROR: Could not open file %s\n", sourceFilePath);
        exit(EXIT_FAILURE);
    }

    DWORD fileSize = GetFileSize(fileHandle, 0);

    result->size = fileSize;
    result->fileName = sourceFilePath;
    result->source = calloc(fileSize + 1, sizeof(char));

    DWORD sizeRead;
    BOOL successfullyReadFile = ReadFile(fileHandle, result->source, fileSize, &sizeRead, 0);

    if (result->source == NULL)
    {
        fprintf(stderr, "ERROR: Could not allocate memory for the source file !\n");
        CloseHandle(fileHandle);
        exit(EXIT_FAILURE);
    }

    if (sizeRead < fileSize)
    {
        fprintf(stderr, "ERROR: Could not read the whole filestream\n");
        free(result->source);
        CloseHandle(fileHandle);
        exit(EXIT_FAILURE);
    }
}
#endif

Lexer InitializeLexer(const char *sourceFilePath)
{
    Lexer result = {0};
    result.line = 1; // NOTE: We start at line one not at line zero in the source file.
#ifdef LINUX_BUILD
    LinuxReadWholeFile(sourceFilePath, &result);
#endif
#ifdef WINDOWS_BUILD
    Win32ReadWholeFile(sourceFilePath, &result);
#endif
    return result;
}

char *TokenString(char *buffer, u64 start, u64 length)
{
    char *result = calloc(length + 1, sizeof(char));

    if (result == NULL)
    {
        fprintf(stderr, "ERROR: Could not allocate memory for Token String\n");
        exit(EXIT_FAILURE);
    }
    char *startOfToken = buffer + start;
    memcpy(result, startOfToken, length);

    return result;
}

char PeekCharacter(Lexer *lexer)
{
    if (lexer->rawPosition >= lexer->size)
    {
        fprintf(stderr, "ERROR: Tried to peek behind the buffer !");
        exit(EXIT_FAILURE);
    }

    return lexer->source[lexer->rawPosition];
}

char ConsumeCharacter(Lexer *lexer)
{
    if (lexer->rawPosition > lexer->size)
    {
        fprintf(stderr, "ERROR: Out of Characters to consume !");
        exit(1);
    }

    char result = lexer->source[lexer->rawPosition];
    lexer->rawPosition += 1;
    lexer->linePosition += 1;

    return result;
}

bool MatchAndConsume(Lexer *lexer, char symbol)
{
    if (lexer->rawPosition < lexer->size && PeekCharacter(lexer) == symbol)
    {
        ConsumeCharacter(lexer);
        return true;
    }

    return false;
}

Token ScanValue(Lexer *lexer)
{
    Token result = {0};
    result.start = lexer->rawPosition;

    while (lexer->rawPosition < lexer->size)
    {
        // NOTE: When we hit a space character we found the end of the value literal.
        char nextCharacter = PeekCharacter(lexer);
        if (isspace(nextCharacter))
        {
            break;
        }

        if (isdigit(nextCharacter))
        {
            ConsumeCharacter(lexer);
        }
        else
        {
            const char *errMessage = "Unknown Symbol in Value literal.";
            ReportError(lexer, errMessage);
        }
    }

    result.length = lexer->rawPosition - result.start;
    result.repr.buffer = TokenString(lexer->source, result.start, result.length);
    result.tt = T_VALUE;
    return result;
}

Token ScanWord(Lexer *lexer)
{
    Token result = {0};
    result.start = lexer->rawPosition;

    while (lexer->rawPosition < lexer->size)
    {
        char nextCharacter = PeekCharacter(lexer);

        if (isspace(nextCharacter))
        {
            break;
        }

        if (islower(nextCharacter) || isupper(nextCharacter))
        {
            ConsumeCharacter(lexer);
        }
        else
        {
            const char *errMessage = "Illegal symbol in word literal. A word can only contain lower and uppercase letters.";
            ReportError(lexer, errMessage);
        }
    }

    result.length = lexer->rawPosition - result.start;
    result.repr.buffer = TokenString(lexer->source, result.start, result.length);
    result.tt = T_WORD;

    return result;
}

void ConsumeComment(Lexer *lexer)
{
    while (lexer->rawPosition < lexer->size)
    {
        if (PeekCharacter(lexer) == ')')
        {
            ConsumeCharacter(lexer);
            break;
        }

        ConsumeCharacter(lexer);
    }
}

Token ScanString(Lexer *lexer)
{
    Token result = {0};
    result.start = lexer->rawPosition;

    while (lexer->rawPosition < lexer->size)
    {
        if (PeekCharacter(lexer) == '"')
        {
            break;
        }

        ConsumeCharacter(lexer);
    }

    if (lexer->rawPosition == lexer->size)
    {
        const char *errMessage =
            "String literal has no end. You need to put a double quote character (\") at the end of your String literal.";
        ReportError(lexer, errMessage);
    }

    ConsumeCharacter(lexer);

    result.length = (lexer->rawPosition - 1) - result.start;
    result.repr.buffer = TokenString(lexer->source, result.start, result.length);
    result.tt = T_STRING;

    return result;
}

Token ScanOperator(Lexer *lexer)
{
    Token result = {0};
    result.start = lexer->rawPosition;
    // TODO: There are symbols with more than one character i.e. comments, we need to handle these as well
    result.length = 1;
    char currentChar = ConsumeCharacter(lexer);
    switch (currentChar)
    {
    case ':':
        result.tt = T_COLON;
        result.repr.symbol = ':';
        break;
    case ';':
        result.tt = T_SEMICOLON;
        result.repr.symbol = ';';
        break;
    case '.':
        if (MatchAndConsume(lexer, '"'))
        {
            result = ScanString(lexer);
        }
        else
        {
            result.tt = T_DOT;
            result.repr.symbol = '.';
        }
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
    {
        const char *errMessage = "Tried to parse a symbol that is not part of the language.";
        ReportError(lexer, errMessage);
    }
    break;
    }

    ConsumeCharacter(lexer);
    return result;
}

void WordToKeyword(Token *token)
{
    if (strcmp(token->repr.buffer, "swap") == 0)
    {
        token->tt = T_SWAP;
    }
    else if (strcmp(token->repr.buffer, "dup") == 0)
    {
        token->tt = T_DUP;
    }
    else if (strcmp(token->repr.buffer, "rot") == 0)
    {
        token->tt = T_ROTATE;
    }
    else if (strcmp(token->repr.buffer, "if") == 0)
    {
        token->tt = T_IF;
    }
    else if (strcmp(token->repr.buffer, "then") == 0)
    {
        token->tt = T_THEN;
    }
    else if (strcmp(token->repr.buffer, "else") == 0)
    {
        token->tt = T_ELSE;
    }
}

TokenList GenerateTokenList(const char *sourcePath)
{
    Lexer lexer = InitializeLexer(sourcePath);
    TokenList tokens = InitializeTokenList();

    while (lexer.rawPosition < lexer.size)
    {
        Token currentToken;
        char currentChar = PeekCharacter(&lexer);

        if (currentChar == '\n')
        {
            lexer.line += 1;
            lexer.linePosition = 0;
        }

        if (islower(currentChar))
        {
            currentToken = ScanWord(&lexer);
            WordToKeyword(&currentToken);
        }
        else if (isdigit(currentChar))
        {
            currentToken = ScanValue(&lexer);
        }
        else if (currentChar == '(')
        {
            ConsumeComment(&lexer);
            continue;
        }
        else if (ispunct(currentChar))
        {
            currentToken = ScanOperator(&lexer);
        }
        else if (isspace(currentChar))
        {
            ConsumeCharacter(&lexer);
            continue;
        }

        PushToken(&tokens, currentToken);
    }

    // NOTE: We free the source buffer here...
    free(lexer.source);
    return tokens;
}