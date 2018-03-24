#include "lexer.h"
#include "parser.h"
#include "engine.h"
#include "database.h"
#include <iostream>
#include <string>
#include <sstream>

int main()
{
    std::ios_base::sync_with_stdio(false);
    Lexer l(std::cin);
    Parser p(l);
    Database db;
    Engine eng(db);
    Command c = {};
    while(c.type != Command::Quit && !std::cin.eof())
    {
        c = p.parseStatement();
        eng.runStatement(c);
    }
}
