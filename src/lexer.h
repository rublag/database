#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <fstream>
#include <utility>
#include <cstddef>

#define LEXER_BUFFER_SIZE 1024

struct Token
{
    enum Type {
        // Commands:
        Select, Insert, Delete, Quit, 
        // Sub:
        Where,
        // Fields:
        Ast, Name, Phone, Value, 
        // Operators:
        Eq, Ne, Le, Ge, Gt, Lt, Like,
        // Logic:
        And, Or,
        // Other:
        EndSt, EndStream, LPar, RPar, ErrLexer, ErrStream, Comma, Nil, Word
    } type;
    char *value;
};

class Lexer
{
    std::istream &input;
    char readBuffer[LEXER_BUFFER_SIZE];
    char wordBuffer[LEXER_BUFFER_SIZE];
    size_t readBufferPosition;
    bool inParentheses = false;

public:
    Lexer() = delete;
    Lexer(const Lexer &) = delete;
    Lexer(Lexer &&) = delete;
    Lexer &operator=(Lexer&&) = delete;
    const Lexer &operator=(const Lexer &) = delete;
    
    Lexer(std::istream &istr) : input(istr)
    {
        readBufferPosition = -1;
    }

    ~Lexer()
    {
    }

    Token nextToken();

private:
    int refillBuffer();
    int getWord(char *&start, char *&end);
    char *createValue();
};

#endif
