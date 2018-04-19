#include "database.h"
#include "test_like.h"

void Database::Iterator::getFirstMatch()
{
    satisfyPredicate();
}

void Database::Iterator::satisfyPredicate()
{
    if(uses == Uses::Name)
    {
        int name_res = 0;
        while(!end && !match(**this, &name_res))
        {
            if(name_res == 0)
                ++names_iterator;
            else if(name_res == -1)
                names_iterator.nextNode();
            else if(name_res == -2)
                end = true;
            
            if(names_iterator.atEnd())
                end = true;
        }
    }
    else
    {
        while(!end)
        {
            if(!groups_iterator.atEnd())
            {
                if( (query.groupOp == Query::Operator::Eq && groups_iterator->group() > query.group) ||
                    (query.groupOp == Query::Operator::Lt && groups_iterator->group() >= query.group) ||
                    (query.groupOp == Query::Operator::Le && groups_iterator->group() > query.group) 
                  )
                {
                    end = true;
                    break;
                }
            }
            if(group_iterator.atEnd())
            {
                ++groups_iterator;
                if(!groups_iterator.atEnd())
                    group_iterator = groups_iterator->select(Database::makeGroupQuery(query));
            }
            if(groups_iterator.atEnd())
                end = true;
            if(!group_iterator.atEnd() && !groups_iterator.atEnd())
            {
                if(match(**this))
                    break;
                ++group_iterator;
            }
        }
    }
}

Database::Iterator::Iterator(ExternalIndex::iterator lower, Query q) : query(q), names_iterator(lower), uses(Uses::Name)
{
    end = names_iterator.atEnd();
    satisfyPredicate();
}

Database::Iterator::Iterator(GroupList::iterator lower, Query q) : query(q), groups_iterator(lower), uses(Uses::Groups)
{
    end = groups_iterator.atEnd();
    if(!end)
        group_iterator = groups_iterator->select(Database::makeGroupQuery(query));
    satisfyPredicate(); 
}

Record &Database::Iterator::operator*()
{
    if(uses == Uses::Name)
        return **names_iterator;
    return *group_iterator;
}

const typename Database::Iterator &Database::Iterator::operator++()
{
    if(uses == Uses::Name)
    {
        ++names_iterator;
        if(names_iterator.atEnd())
            end = true;
        satisfyPredicate();
    }
    else
    {
        if(!group_iterator.atEnd())
            ++group_iterator;
        satisfyPredicate();
    }
    return *this;
}

bool Database::Iterator::match(const Record &record, int *name_res) const
{
    if(name_res)
        *name_res = 0;

    switch(query.nameOp)
    {
    case Query::Operator::Nil:
        break;
    case Query::Operator::Eq:
    {
        auto cmp = std::strcmp(query.name, record.name());
        if(cmp < 0)
        {
            if(name_res)
            {
                *name_res = -1;
            }
            return false;
        }
        else if(cmp > 0)
        {
            if(name_res)
                *name_res = -2;
            return false;
        }
        break;
    }
    case Query::Operator::Ne:
    {
        auto cmp = std::strcmp(query.name, record.name());
        if(cmp == 0)
        {
            if(name_res)
            {
                *name_res = -1;
            }
            return false;
        }
        break;
    }
    case Query::Operator::Lt:
    {
        auto cmp = std::strcmp(query.name, record.name());
        if(cmp >= 0)
        {
            if(name_res)
                *name_res = -2;
            return false;
        }
        break;
    }
    case Query::Operator::Le:
    {
        auto cmp = std::strcmp(query.name, record.name());
        if(cmp > 0)
        {
            if(name_res)
                *name_res = -2;
            return false;
        }
        break;
    }
    case Query::Operator::Gt:
    {
        auto cmp = std::strcmp(query.name, record.name());
        if(cmp <= 0)
        {
            if(name_res)
            {
                *name_res = -1;
            }
            return false;
        }
        break;
    }
    case Query::Operator::Ge:
    {
        auto cmp = std::strcmp(query.name, record.name());
        if(cmp < 0)
        {
            if(name_res)
            {
                *name_res = -1;
            }
            return false;
        }
        break;
    }
    case Query::Operator::Like:
    {
        if(!test_like(record.name(), query.name))
            return false;
        break;
    }
    default:
        return false;
    }

    switch(query.phoneOp)
    {
    case Query::Operator::Nil:
        break;
    case Query::Operator::Eq:
        if(query.phone != record.phone())
            return false;
        break;
    case Query::Operator::Ne:
        if(query.phone == record.phone())
            return false;
        break;
    case Query::Operator::Lt:
        if(query.phone >= record.phone())
            return false;
        break;
    case Query::Operator::Le:
        if(query.phone > record.phone())
            return false;
        break;
    case Query::Operator::Gt:
        if(query.phone <= record.phone())
            return false;
        break;
    case Query::Operator::Ge:
        if(query.phone < record.phone())
            return false;
        break;
    default:
        break;
    }

    switch(query.groupOp)
    {
    case Query::Operator::Nil:
        break;
    case Query::Operator::Eq:
        if(query.group != record.group())
            return false;
        break;
    case Query::Operator::Ne:
        if(query.group == record.group())
            return false;
        break;
    case Query::Operator::Lt:
        if(query.group >= record.group())
            return false;
        break;
    case Query::Operator::Le:
        if(query.group > record.group())
            return false;
        break;
    case Query::Operator::Gt:
        if(query.group <= record.group())
            return false;
        break;
    case Query::Operator::Ge:
        if(query.group < record.group())
            return false;
        break;
    default:
        break;
    }

    return true;
}

bool Database::Iterator::atEnd()
{
    return end;
}
