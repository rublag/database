#include "group.h"
#include "test_like.h"

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

bool Group::Iterator::match(const Record &record, int *name_res) const
{
    if(name_res)
        *name_res = 0;

    switch(query.nameOp)
    {
    case Query::Operator::Nil:
        break;
    case Query::Operator::Eq:
    {
        auto cmp = std::strcmp(record.name(), query.name);
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
        auto cmp = std::strcmp(record.name(), query.name);
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
        auto cmp = std::strcmp(record.name(), query.name);
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
        auto cmp = std::strcmp(record.name(), query.name);
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
        auto cmp = std::strcmp(record.name(), query.name);
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
        auto cmp = std::strcmp(record.name(), query.name);
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

    return true;
}

bool Group::Iterator::atEnd()
{
    return end;
}
