#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "command.h"

#define PARSER_BUFFER_SIZE LEXER_BUFFER_SIZE*3
class Parser
{
private:
    Lexer &lexer;
    char textBuffer[PARSER_BUFFER_SIZE];
    size_t textBufferPosition = 0;

public:
    Parser() = delete;
    Parser(const Parser&) = delete;
    Parser(Parser&&) = delete;
    const Parser &operator=(const Parser&) = delete;
    Parser &operator=(Parser&&) = delete;

    Parser(Lexer &lexer) : lexer(lexer) {}
    Command parseStatement();

private:
    bool parseWhere(Command &command);
    char *copyToBuffer(char*);
};

#endif
