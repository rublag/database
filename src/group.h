#ifndef GROUP_H
#define GROUP_H


#include "name_index.h"
#include "record.h"
#include "records.h"

class Group : private RecordList
{
    int _group = 0;
    NameIndex  names;

public:
    using RecordList::iterator;
    using RecordList::begin;
    using RecordList::end;

    int group()
    {
        return _group;
    }

    iterator insert(Record &&record)
    {
        auto it = records.insert(std::move(record));
        names.index(it);
        return it;
    }

    iterator erase(iterator q)
    {
        auto next = q;
        ++next;
        names.erase(*q);
        RecordList::erase(q);
        return next;
    }

    iterator erase(iterator q1, iterator q2)
    {
        while(q1 != q2)
        {
            q1 = erase(q1);
        }
    }

    iterator erase(NameIndex::iterator q)
    {
        auto next = q;
        ++next;
        auto r = *q;
        names.erase(q);
        RecordList::erase(r);
        return next;
    }
    
    NameIndex::iterator lower_bound_name(char *name)
    {
        return names.lower_bound(name);
    }

    NameIndex::iterator upper_bound_name(char *name)
    {
        return names.upper_bound(name);
    }

    NameIndex::iterator begin_name(char *name)
    {
        return names.begin();
    }

    NameIndex::iterator end_name(char *name)
    {
        return names.end();
    }
};

bool operator==(Group &g, int i)
{
    return g.group() == i;
}

bool operator==(Group &g1, Group &g2)
{
    return g1.group() == g2.group();
}

bool operator==(int i, Group &g)
{
    return g == i;
}

#endif
