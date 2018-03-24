#include <iostream>

#include "engine.h"
#include "command.h"
#include "database.h"
#include "record.h"

static bool test_record(const Record &record, const Command &command)
{
    return false;
    // len' pisat'
}

static void print_record(const Record &record, const Command &command)
{
    if(command.ast)
    {
        std::cout << " Name: " << record.name() << " Group: " << record.group() << " Phone: " << record.phone() << std::endl;
        return;
    }

    for(int i = 0; i < 3; ++i)
    {
        switch(command.query[i])
        {
        case Command::Name:
            std::cout << " Name: " << record.name();
            break;
        case Command::Value:
            std::cout << " Group: " << record.group();
            break;
        case Command::Phone:
            std::cout << " Phone: " << record.phone();
            break;
        default:
            break;
        }
    }
    std::cout << std::endl;
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
    }
}

void Engine::runStatement(const Command &command)
{
    // dlist of groups. each groups contains container with records, avl for name and linear search for phone. Global avl for name.

    DatabaseQuery db_query;

    switch(command.type)
    {
    case Command::Insert:
        insert(command);
        break;
    case Command::Delete:
        remove(command);
        break;
    case Command::Select:
        select(command);
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

static DatabaseQuery make_db_query(const Command &command)
{
    DatabaseQuery query;
    for(int i = 0; i < 3; ++i)
    {
        DatabaseQuery::Operator tmp;
        switch(command.cond[i].type)
        {
        case Condition::Eq:
            tmp = DatabaseQuery::Operator::Eq;
            break;
        case Condition::Gt:
            tmp = DatabaseQuery::Operator::Gt;
            break;
        case Condition::Lt:
            tmp = DatabaseQuery::Operator::Lt;
            break;
        case Condition::Ge:
            tmp = DatabaseQuery::Operator::Ge;
            break;
        case Condition::Le:
            tmp = DatabaseQuery::Operator::Le;
            break;
        case Condition::Ne:
            tmp = DatabaseQuery::Operator::Ne;
            break;
        case Condition::Like:
            tmp = DatabaseQuery::Operator::Ge;
            break;
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
        }
    }
}

void Engine::select(const Command &command)
{
    // dlist of groups. each groups contains container with records, avl for name and linear search for phone. Global avl for name.

    if(command.cond[3].left != Condition::Left::Nil)
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
                cmd2.cond[1] = {};
            }
            else
            {
                cmd1.andSet[0] = 1;
                invert(cmd1.cond[0]);
                cmd2.cond[1] = {};
            }
        }
        else
        {
            cmd1.andSet[0] = 1;
            invert(cmd1.cond[1]);
            cmd2.cond[0] = cmd2.cond[1];
            cmd2.cond[1] = {};
        }

        select(cmd1);
        select(cmd2);
    }

    auto db_query = make_db_query(command);
    for(auto db_cursor = database.query(db_query); !db_cursor.end(); db_cursor.next())
    {
        auto &record = db_cursor.get();
        if(test_record(record, command))
            print_record(record, command);
    }
}
