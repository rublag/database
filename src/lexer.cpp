#include "lexer.h"

#include <cstring>

int Lexer::refillBuffer()
{
    readBufferPosition = -1;
    input.getline(readBuffer, LEXER_BUFFER_SIZE-1);
    if(input.eof())
    {
        if(input.gcount())
        {
            readBufferPosition = 0;
            return 1;
        }
        return -1;
    }
    if(!input.fail())
    {
        readBuffer[input.gcount()-1] = '\n';
        readBuffer[input.gcount()] = '\0';
        readBufferPosition = 0;
        return 1;
    }
    else if(input.gcount())
    {
        readBuffer[input.gcount()] = '\n';
        readBuffer[input.gcount()+1] = '\0';
        readBufferPosition = 0;
        input.clear();
        return 1;
    }
    else
        return 0;
    readBufferPosition = 0;
    return 1;
}

// Possible Token::Type values and lexems.
// SELECT -> Select
// insert -> Insert
// delete -> Delete
// quit   -> Quit
//
// where  -> Where
// 
// *      -> Ast
// name   -> Name
// phone  -> Phone
// value  -> Value
//
// <=     -> Le
// <      -> Lt
// >=     -> Ge
// >      -> Gt
// =      -> Eq
// <>     -> Ne
// like   -> Like
//
// and    -> And
// or     -> Or
//
// ;      -> EndSt
// ,      -> Comma
// (      -> LPar
// )      -> RPar
//
// 
// Other:    Word
// Errors: ErrorLexer, ErrStream

Token Lexer::nextToken()
{
    char *start, *end;

    Token token;
    token.type = Token::Nil;
    token.value = nullptr;

    int r = getWord(start, end);
    if(r == -1)
    {
        token.type = Token::EndStream;
        return token;
    }

    else if(r == 0)
    {
        token.type = Token::ErrStream;
        return token;
    }

    char *word = start;
    size_t len = end - start;

    if(!len)
    {
        token.type = Token::EndStream;
        token.value = nullptr;
        return token;
    }
    if(!inParentheses && len == 6 && strncmp(word, "select", 6) == 0)
    {
        token.type = Token::Select;
        token.value = nullptr;
        return token;
    }
    else if(!inParentheses && len == 6 && strncmp(word, "insert", 6) == 0)
    {
        token.type = Token::Insert;
        token.value = nullptr;
        return token;
    }
    else if(!inParentheses && len == 6 && strncmp(word, "delete", 6) == 0)
    {
        token.type = Token::Delete;
        token.value = nullptr;
        return token;
    }
    else if(!inParentheses && len == 4 && strncmp(word, "quit", 4) == 0)
    {
        token.type = Token::Quit;
        token.value = nullptr;
        return token;
    }
    else if(!inParentheses && len == 5 && strncmp(word, "where", 5) == 0)
    {
        token.type = Token::Where;
        token.value = nullptr;
        return token;
    }
    else if(len == 1 && strncmp(word, "*", 1) == 0)
    {
        token.type = Token::Ast;
        token.value = nullptr;
        return token;
    }
    else if(!inParentheses && len == 4 && strncmp(word, "name", 4) == 0)
    {
        token.type = Token::Name;
        token.value = nullptr;
        return token;
    }
    else if(!inParentheses && len == 5 && strncmp(word, "phone", 5) == 0)
    {
        token.type = Token::Phone;
        token.value = nullptr;
        return token;
    }
    else if(!inParentheses && len == 5 && strncmp(word, "group", 5) == 0)
    {
        token.type = Token::Value;
        token.value = nullptr;
        return token;
    }
    else if(!inParentheses && len == 3 && strncmp(word, "and", 3) == 0)
    {
        token.type = Token::And;
        token.value = nullptr;
        return token;
    }
    else if(!inParentheses && len == 2 && strncmp(word, "or", 2) == 0)
    {
        token.type = Token::Or;
        token.value = nullptr;
        return token;
    }
    else if(len == 1 && strncmp(word, ",", 1) == 0)
    {
        token.type = Token::Comma;
        token.value = nullptr;
        return token;
    }
    else if(len == 1 && strncmp(word, ";", 1) == 0)
    {
        token.type = Token::EndSt;
        token.value = nullptr;
        return token;
    }
    else if(len == 1 && strncmp(word, "(", 1) == 0)
    {
        token.type = Token::LPar;
        token.value = nullptr;
        inParentheses = true;
        return token;
    }
    else if(len == 1 && strncmp(word, ")", 1) == 0)
    {
        token.type = Token::RPar;
        token.value = nullptr;
        inParentheses = false;
        return token;
    }
    // Operators:
    else if(len == 1 && strncmp(word, "=", 1) == 0)
    {
        token.type = Token::Eq;
        if(!(token.value = createValue()))
            token.type = Token::ErrLexer;
        return token;
    }
    else if(len == 2 && strncmp(word, "<>", 2) == 0)
    {
        token.type = Token::Ne;
        if(!(token.value = createValue()))
            token.type = Token::ErrLexer;
        return token;
    }
    else if(len == 1 && strncmp(word, "<", 1) == 0)
    {
        token.type = Token::Lt;
        if(!(token.value = createValue()))
            token.type = Token::ErrLexer;
        return token;
    }
    else if(len == 2 && strncmp(word, "<=", 2) == 0)
    {
        token.type = Token::Le;
        if(!(token.value = createValue()))
            token.type = Token::ErrLexer;
        return token;
    }
    else if(len == 1 && strncmp(word, ">", 1) == 0)
    {
        token.type = Token::Gt;
        if(!(token.value = createValue()))
            token.type = Token::ErrLexer;
        return token;
    }
    else if(len == 2 && strncmp(word, ">=", 2) == 0)
    {
        token.type = Token::Ge;
        if(!(token.value = createValue()))
            token.type = Token::ErrLexer;
        return token;
    }
    else if(!inParentheses && len == 4 && strncmp(word, "like", 4) == 0)
    {
        token.type = Token::Like;
        if(!(token.value = createValue()))
            token.type = Token::ErrLexer;
        return token;
    }
    else
    {
        token.value = start;
        token.type = Token::Word;
        return token;
    }
}

int Lexer::getWord(char *&startPtr, char *&endPtr)
{
    if(readBufferPosition >= LEXER_BUFFER_SIZE || !readBuffer[readBufferPosition])
    {
        auto r = refillBuffer();
        if(r == -1)
            return -1;
        if(!r)
            return 0;
    }
    // Only Separators are \n, \t, space, comma, parentheses, semicolon.
    // Pure whitespaces are \n, \t, space

    size_t word_buffer_pos = 0;

    size_t start = readBufferPosition;
    while(readBuffer[start] == '\t' || readBuffer[start] == '\n' || readBuffer[start] == ' ' || readBuffer[start] == '\0')
    {
        if(start >= LEXER_BUFFER_SIZE-1 || !readBuffer[start])
        {
            auto r = refillBuffer();
            if(r == -1)
                return -1;
            if(!r)
                return 0;
            start = readBufferPosition;
        }
        else
            ++start;
    }

    size_t end = start;
    while(!(readBuffer[end] == '\t' || readBuffer[end] == '\n' || readBuffer[end] == ' ' ||
            readBuffer[end] == '(' || readBuffer[end] == ')' || readBuffer[end] == ',' || readBuffer[end] == ';'
           )
         )
    {
        if(word_buffer_pos >= LEXER_BUFFER_SIZE-1)
            return 0;
        if(end >= LEXER_BUFFER_SIZE-1 || !readBuffer[end])
        {
            auto r = refillBuffer();
            end = readBufferPosition;
            if(r == -1)
                break;
            if(!r)
                return 0;
        }
        else
        {
            wordBuffer[word_buffer_pos++] = readBuffer[end++];
        }
    }

    if(word_buffer_pos == 0 && ( readBuffer[end] == '(' || readBuffer[end] == ')' || readBuffer[end] == ',' || readBuffer[end] == ';'))
    {
        wordBuffer[word_buffer_pos++] = readBuffer[end++];
    }
    wordBuffer[word_buffer_pos] = '\0';
    startPtr = wordBuffer;
    endPtr = wordBuffer + word_buffer_pos;
    readBufferPosition = end;
    return 1;
}

char *Lexer::createValue()
{
    char *start, *end;
    auto r = getWord(start, end);
    if(r == -1)
        return nullptr;
    if(r == 0)
        return nullptr;
    return start;
}
