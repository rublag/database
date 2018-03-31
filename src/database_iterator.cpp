#include "database.h"

void Database::Iterator::getFirstMatch()
{
    if(!end && !match(*(*this)))
        ++(*this);
}

Database::Iterator::Iterator(ExternalIndex::iterator lower, Query q) : names_iterator(lower), query(q), uses(Uses::Name)
{
    getFirstMatch();
}

Database::Iterator::Iterator(GroupList::iterator lower, Query q) : groups_iterator(lower), query(q), uses(Uses::Groups)
{
    getFirstMatch();
}

Record &Database::Iterator::operator*()
{
    if(uses == Uses::Name)
        return *names_iterator;
    return *group_iterator;
}

const typename Database::Iterator &Database::Iterator::operator++()
{
    if(uses == Uses::Name)
    {
        while(!end && !match(*(*this)))
        {
            ++names_iterator;
            if(names_iterator.atEnd())
                end = true;
        }
    }
    else
    {
        while(!end && !match(*(*this)))
        {
            if(!group_iterator.atEnd())
                ++group_iterator;
            else
            {
                ++groups_iterator;
                group_iterator = groups_iterator->search(make_query(query));
            }
            if(groups_iterator.atEnd() && group_iterator.atEnd())
                end = true;
        }
    }
    return *this;
}

bool Database::Iterator::match(const Record &record) const
{
    switch(query.nameOp)
    {
    case Query::Operator::Nil:
        break;
    case Query::Operator::Eq:
        if(std::strcmp(query.name, record.name()) != 0)
            return false;
        break;
    case Query::Operator::Ne:
        if(std::strcmp(query.name, record.name()) == 0)
            return false;
        break;
    case Query::Operator::Lt:
        if(std::strcmp(query.name, record.name()) >= 0)
            return false;
        break;
    case Query::Operator::Le:
        if(std::strcmp(query.name, record.name()) > 0)
            return false;
        break;
    case Query::Operator::Gt:
        if(std::strcmp(query.name, record.name()) <= 0)
            return false;
        break;
    case Query::Operator::Ge:
        if(std::strcmp(query.name, record.name()) < 0)
            return false;
        break;
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
    }

    return true;
}
