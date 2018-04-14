#include "group.h"

void Group::Iterator::getFirstMatch()
{
    satisfyPredicate();
}

void Group::Iterator::satisfyPredicate()
{
    while(!end && !match(**this))
    {
        if(uses == Uses::Name)
        {
            ++names_iterator;
            if(names_iterator.atEnd())
                end = true;
        }
        else
        {
            ++records_iterator;
            if(records_iterator.atEnd())
                end = true;
        }
    }
}

Group::Iterator::Iterator() : uses(Uses::Nil) {}

Group::Iterator::Iterator(InternalIndex::iterator lower, Query q) : query(q), names_iterator(lower), uses(Uses::Name)
{
    end = names_iterator.atEnd();
    satisfyPredicate();
}

Group::Iterator::Iterator(RecordList::iterator lower, Query q) : query(q), records_iterator(lower), uses(Uses::Records)
{
    end = records_iterator.atEnd();
    satisfyPredicate();
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
        ++names_iterator;
        if(names_iterator.atEnd())
            end = true;
    }
    else
    {
        ++records_iterator;
        if(records_iterator.atEnd())
            end = true;
    }
    satisfyPredicate();
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
