#include <iostream>
#include <ostream>

#include "engine.h"
#include "command.h"
#include "database.h"
#include "record.h"

static bool test_record(const Record &, const Command &)
{
    return true;
}

static void print_record(const Record &record, const Command &command, std::ostream &ostr = std::cout)
{
    if(command.ast)
    {
        ostr << " " << record.name() << " " << record.group() << " " << record.phone() << std::endl;
        return;
    }

    for(int i = 0; i < 3; ++i)
    {
        switch(command.query[i])
        {
        case Command::Name:
            ostr << " " << record.name();
            break;
        case Command::Value:
            ostr << " " << record.group();
            break;
        case Command::Phone:
            ostr << " " << record.phone();
            break;
        default:
            break;
        }
    }
    ostr << std::endl;
}

static void invert(Condition &cond)
{
    switch(cond.type)
    {
    case Condition::Eq:
        cond.type = Condition::Ne;
        break;
    case Condition::Ne:
        cond.type = Condition::Eq;
        break;
    case Condition::Lt:
        cond.type = Condition::Ge;
        break;
    case Condition::Le:
        cond.type = Condition::Gt;
        break;
    case Condition::Gt:
        cond.type = Condition::Le;
        break;
    case Condition::Ge:
        cond.type = Condition::Lt;
        break;
    default:
        break;
    }
}

void Engine::runStatement(const Command &command)
{
    // dlist of groups. each groups contains container with records, avl for name and linear search for phone. Global avl for name.
    switch(command.type)
    {
    case Command::Insert:
        insert(command);
        break;
    case Command::Delete:
        action(command, true);
        break;
    case Command::Select:
        action(command, false);
        break;
    case Command::Quit:
        this->~Engine();
        break;
    default:
        break;
    }
    return;
}

void Engine::insert(const Command &command)
{
    auto phone = command.phone;
    auto group = command.value;
    auto name  = command.name;

    auto record = Record(name, group, phone);

    database.insert(std::move(record));
}

static bool command_has_cond(const Command &command, Condition::Left left, Condition::Type type)
{
    if( (command.cond[0].left == left && command.cond[0].type == type)
      ||(command.cond[1].left == left && command.cond[1].type == type)
      )
    {
      return true;
    }
    return false;
}

static Database::Query make_db_query(const Command &command)
{
    Database::Query query = Database::Query();
    for(int i = 0; i < 3; ++i)
    {
        Database::Query::Operator tmp = Database::Query::Operator::Nil;
        switch(command.cond[i].type)
        {
        case Condition::Eq:
            tmp = Database::Query::Operator::Eq;
            break;
        case Condition::Gt:
            tmp = Database::Query::Operator::Gt;
            break;
        case Condition::Lt:
            tmp = Database::Query::Operator::Lt;
            break;
        case Condition::Ge:
            tmp = Database::Query::Operator::Ge;
            break;
        case Condition::Le:
            tmp = Database::Query::Operator::Le;
            break;
        case Condition::Ne:
            tmp = Database::Query::Operator::Ne;
            break;
        case Condition::Like:
            tmp = Database::Query::Operator::Ge;
            break;
        case Condition::Nil:
            tmp = Database::Query::Operator::Nil;
        }

        switch(command.cond[i].left)
        {
        case Condition::Left::Name:
            query.nameOp = tmp;
            query.name = command.cond[i].str;
            break;
        case Condition::Left::Value:
            query.groupOp = tmp;
            query.group = command.cond[i].value;
            break;
        case Condition::Left::Phone:
            query.phoneOp = tmp;
            query.phone = command.cond[i].value;
            break;
        default:
            break;
        }
    }
    return query;
}

void Engine::action(const Command &command, bool remove)
{
    // dlist of groups. each groups contains container with records, avl for name and linear search for phone. Global avl for name.

    if(command.cond[2].left != Condition::Left::Nil)
    {
        // Unsupported
        std::cerr << "3 arguments for select are not supported at the moment." << std::endl;
        return;
    }

    // Check if conditions are specified on different fields
    if( (command.cond[0].left == command.cond[1].left) && command.cond[1].left != Condition::Left::Nil)
    {
        std::cerr << "Conditions must be specified on the different fields." << std::endl;
        return;
    }

    // If there's an OR operator

    if(command.cond[1].left != Condition::Left::Nil && !command.andSet[0])
    {
        // a || b -> a&&!b, b
        auto cmd1 = command;
        auto cmd2 = command;

        if(command_has_cond(command, Condition::Left::Name, Condition::Like))
        {
            if(command.cond[0].left == Condition::Left::Name)
            {
                cmd1.andSet[0] = 1;
                invert(cmd1.cond[1]);
                cmd2.cond[0] = cmd2.cond[1];
                cmd2.cond[1] = Condition();
            }
            else
            {
                cmd1.andSet[0] = 1;
                invert(cmd1.cond[0]);
                cmd2.cond[1] = Condition();
            }
        }
        else
        {
            cmd1.andSet[0] = 1;
            invert(cmd1.cond[1]);
            cmd2.cond[0] = cmd2.cond[1];
            cmd2.cond[1] = Condition();
        }

        action(cmd1, remove);
        action(cmd2, remove);
        return;
    }

    auto query = make_db_query(command);
    if(!remove)
    {
        for(auto it = database.select(query); !it.atEnd(); ++it)
        {
            auto &record = *it;
            if(test_record(record, command))
                print_record(record, command, ostr);
        }
    }
    else
    {
        database.remove(query);
    }
}

Engine::Engine(Database &db, std::ostream &ostr) : ostr(ostr), database(db) {}
