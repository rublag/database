#include "parser.h"
#include <cstring>

Command Parser::parseStatement()
{
    // Expecting that we are at the beginning of the statement (next token is action).
    textBufferPosition = 0;
    Command command = {};
    auto token = lexer.nextToken();

    switch(token.type)
    {
    case Token::Select:
    {
        // [select]
        command.type = Command::Select;

        // select [*]
        token = lexer.nextToken();
        if(token.type == Token::Ast)
        {
            command.ast = true;
            token = lexer.nextToken();
        }

        // select [name/phone/...]
        else
        {
            for(int i = 0; i < 3; ++i)
            {
                if(token.type == Token::Name)
                    command.query[i] = Command::Field::Name;
                else if(token.type == Token::Phone)
                    command.query[i] = Command::Field::Phone;
                else if(token.type == Token::Value)
                    command.query[i] = Command::Field::Value;
                else
                {
                    command.type = Command::ErrParser;
                    return command;
                }
                
                token = lexer.nextToken();
                if(token.type == Token::Comma)
                {
                    token = lexer.nextToken();
                }
                else
                    break;
            }
        }

        // select WHAT [where] or select WHAT;
        if(token.type == Token::EndSt)
        {
            return command;
        }
        if(token.type != Token::Where)
        {
            command.type = Command::ErrParser;
            return command;
        }
        if(!parseWhere(command))
        {
            command.type = Command::ErrParser;
            return command;
        }
        return command;
    }
    case Token::Insert:
    {
        // [insert]
        command.type = Command::Insert;
        token = lexer.nextToken();
        // [insert][(]
        if(token.type != Token::LPar)
        {
            command.type = Command::ErrParser;
            return command;
        }
        // [insert][(]<name>
        token = lexer.nextToken();
        if(token.type != Token::Word)
        {
            command.type = Command::ErrParser;
            return command;
        }
        command.name = copyToBuffer(token.value);

        // [insert][(]<name>[,]
        token = lexer.nextToken();
        if(token.type != Token::Comma)
        {
            command.type = Command::ErrParser;
            return command;
        }

        // [insert][(]<name>[,]<phone>
        token = lexer.nextToken();
        if(token.type != Token::Word)
        {
            command.type = Command::ErrParser;
            return command;
        }
        char *endptr;
        long value = strtol(token.value, &endptr, 10);
        if(endptr == token.value || *endptr != '\0')
        {
            command.type = Command::ErrParser;
            return command;
        }
        command.phone = value;

        // [insert][(]<name>[,]<phone>[,]
        token = lexer.nextToken();
        if(token.type != Token::Comma)
        {
            command.type = Command::ErrParser;
            return command;
        }

        // [insert][(]<name>[,]<phone>[,]<value>
        token = lexer.nextToken();
        if(token.type != Token::Word)
        {
            command.type = Command::ErrParser;
            return command;
        }
        value = strtol(token.value, &endptr, 10);
        if(endptr == token.value || *endptr != '\0')
        {
            command.type = Command::ErrParser;
            return command;
        }
        command.value = value;

        // [insert][(]<name>[,]<phone>[,]<value>[)]
        token = lexer.nextToken();
        if(token.type != Token::RPar)
        {
            command.type = Command::ErrParser;
            return command;
        }

        // [insert][(]<name>[,]<phone>[,]<value>[)][;]
        token = lexer.nextToken();
        if(token.type != Token::EndSt)
        {
            command.type = Command::ErrParser;
            return command;
        }
        return command;
    }
    case Token::Delete:
    {
        // [delete]
        command.type = Command::Delete;

        token = lexer.nextToken();
        
        // [delete][;]
        if(token.type == Token::EndSt)
            return command;

        // [delete][where]
        if(token.type != Token::Where)
        {
            command.type = Command::ErrParser;
            return command;
        }

        if(!parseWhere(command))
        {
            command.type = Command::ErrParser;
            return command;
        }
        return command;
    }
    case Token::Quit:
    {
        // [quit]
        command.type = Command::Quit;

        // [quit][;]
        token = lexer.nextToken();
        if(token.type != Token::EndSt)
        {
            command.type = Command::ErrParser;
            return command;
        }
        return command;
    }
    default:
    {
        command.type = Command::ErrParser;
        break;
    }
    }
    return command;
}

bool strtoint(char *str, int &res)
{
    char *endptr;
    long v = strtol(str, &endptr, 10);
    if(endptr == str || *endptr != '\0')
        return false;
    res = v;
    return true;
}

bool Parser::parseWhere(Command &command)
{
    for(int i = 0; i < 3; ++i)
    {
        auto token = lexer.nextToken();
        bool int_field = false;
        // ... [where]
        // ... [where]<field>
        if(token.type == Token::Name)
        {
            command.cond[i].left = Condition::Left::Name;
        }
        else if(token.type == Token::Phone)
        {
            command.cond[i].left = Condition::Left::Phone;
            int_field = true;
        }
        else if(token.type == Token::Value)
        {
            command.cond[i].left = Condition::Left::Value;
            int_field = true;
        }
        else
        {
            return false;
        }

        // ... [where]<field><operator>
        token = lexer.nextToken();
        switch(token.type)
        {
        case Token::Eq:
        {
            command.cond[i].type = Condition::Eq;
            break;
        }
        case Token::Ne:
        {
            command.cond[i].type = Condition::Ne;
            break;
        }
        case Token::Lt:
        {
            command.cond[i].type = Condition::Lt;
            break;
        }
        case Token::Le:
        {
            command.cond[i].type = Condition::Le;
            break;
        }
        case Token::Gt:
        {
            command.cond[i].type = Condition::Gt;
            break;
        }
        case Token::Ge:
        {
            command.cond[i].type = Condition::Ge;
            break;
        }
        case Token::Like:
        {
            command.cond[i].type = Condition::Like;
            break;
        }
        default:
        {
            return false;
        }
        }

        if(int_field && !strtoint(token.value, command.cond[i].value))
            return false;
        else if(!int_field)
        {
            command.cond[i].str = copyToBuffer(token.value);
        }

        token = lexer.nextToken();
        if(token.type == Token::And)
        {
            command.andSet[i] = true;
        }
        else if(token.type == Token::Or)
        {
            command.andSet[i] = false;
        }
        else if(token.type == Token::EndSt)
        {
            return true;
        }
    }
    auto token = lexer.nextToken();
    if(token.type != Token::EndSt)
    {
        return false;
    }
    return true;
}

char *Parser::copyToBuffer(char *src)
{
    if(!src)
        return src;

    if(textBufferPosition >= PARSER_BUFFER_SIZE)
        return nullptr;

    auto len = strlen(src);
    if(len >= PARSER_BUFFER_SIZE - textBufferPosition)
        return nullptr;

    auto dst = textBuffer+textBufferPosition;
    strcpy(dst, src);
    textBufferPosition += len+1;
    return dst;
}
