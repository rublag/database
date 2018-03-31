#include "group.h"

int Group::group() const
{
    return _group;
}

Record &Group::insert(Record &&record)
{
    auto it = records.insert(std::move(record));
    names.insert(std::move(it));
    return *it;
}

Group::iterator Group::select(Query query) const
{
    if(query.nameOp != Query::Operator::Nil)
    {
        switch(query.nameOp)
        {
        case Query::Operator::Eq:
        case Query::Operator::Gt:
        case Query::Operator::Ge:
        {
            auto it_inner = names.lower_bound(query.name);
            auto it = Iterator(it_inner, query);
            return it;
        }
        
        case Query::Operator::Ne:
        case Query::Operator::Lt:
        case Query::Operator::Le:
        default:
        {
            auto it_inner = names.begin();
            auto it = Iterator(it_inner, query);
            return it;
        }
        }
    }

    auto it_inner = records.begin();
    auto it = Iterator(it_inner, query);
    return it;
}

typename Group::iterator Group::erase(Group::iterator s)
{
    auto tmp = s;
    ++s;
    
    if(s.uses == Iterator::Uses::Name)
    {
        auto it = s.names_iterator;
        records.erase(*it);
        names.erase(it);
    }
    else
    {
        names.erase(s.records_iterator);
        records.erase(s.records_iterator);
    }
    return tmp;
}

void Group::erase(const Record &record)
{
    auto it = names.find(record);
    auto lower = *it;
    names.erase(it);
    records.erase(lower);
}

Group::Group(int g) : _group(g) {}

bool operator==(const Group &g, int i)
{
    return g.group() == i;
}

bool operator==(const Group &g1, const Group &g2)
{
    return g1.group() == g2.group();
}

bool operator==(int i, const Group &g)
{
    return g == i;
}


