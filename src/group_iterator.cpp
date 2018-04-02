#include "group.h"

void Group::Iterator::getFirstMatch()
{
    if(!end && !match(*(*this)))
        ++(*this);
}

Group::Iterator::Iterator() : uses(Uses::Nil) {}

Group::Iterator::Iterator(InternalIndex::iterator lower, Query q) : query(q), names_iterator(lower), uses(Uses::Name)
{
    getFirstMatch();
}

Group::Iterator::Iterator(RecordList::iterator lower, Query q) : query(q), records_iterator(lower), uses(Uses::Records)
{
    getFirstMatch();
}

Record &Group::Iterator::operator*()
{
    if(uses == Uses::Name)
        return **names_iterator;
    return *records_iterator;
}

const typename Group::Iterator &Group::Iterator::operator++()
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
            ++records_iterator;
            if(records_iterator.atEnd())
                end = true;
        }
    }
    return *this;
}

bool Group::Iterator::match(const Record &record) const
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

bool Group::Iterator::atEnd()
{
    return end;
}