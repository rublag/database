#include "lexer.h"
#include "parser.h"
#include "engine.h"
#include "database.h"
#include <iostream>
#include <string>
#include <sstream>

int main(int, char *argv[])
{
    //std::ios_base::sync_with_stdio(false);
    Lexer l(std::cin);
    Parser p(l);
    std::ifstream istr;
    istr.open(argv[1], std::ifstream::in);
    Database db(istr);
    Engine eng(db);
    Command c = {};
    while(c.type != Command::Quit && !std::cin.eof())
    {
        //std::cout << ">>> ";
        c = p.parseStatement();
        if(c.type == Command::ErrParser)
        {
            std::cout << "  Invalid command!" << std::endl;
        }
        else
            eng.runStatement(c);
    }
}
