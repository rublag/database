#ifndef GROUP_H
#define GROUP_H


#include "name_index.h"
#include "record.h"
#include "records.h"

class Group
{
    class IteratorRecord;
    class IteratorName;
    int _group = 0;
    NameIndex  names;

public:
    struct Query;
    class Iterator;
    using iterator = Iterator;

    int group()
    {
        return _group;
    }

    RecordList::iterator insert(Record &&record)
    {
        auto it = records.insert(std::move(record));
        names.index(it);
        return it;
    }
    
    iterator select(Query query)
    {
        if(!query.nameOp)
        {
            return IteratorRecord(records.begin(), query)
        }
        return IteratorName(names.begin(), query);
    }

    iterator remove(iterator s)
    {
        auto tmp = s;
        ++s;
        names.erase(s.index());
        records.erase(s.record());
    }
    
    Group(int g) : _group(g) {}
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

struct Group::Query
{
    enum class Operator {
        Nil = 0, Eq, Ne, Gt, Ge, Lt, Le
    } nameOp, phoneOp;
    char *name;
    int  phone;
};

class Group::Iterator
{
    Group::Query query;
public:
    virtual const Iterator &operator++() =0;
    virtual Record &operator*()          =0;
    virtual NameIndex::iterator index()  =0;
    virtual RecordList::iterator record()=0;
};

class Group::IteratorName : public Group::Iterator
{
    NameIndex::iterator it;
public:
    virtual const IteratorName &operator++()
    {
        while(!end(it))
        {
            ++it;
            if(test(*it, query))
            {
                return *this;
            }
        }
        return *this;
    }
    
    virtual Record &operator*
    {
        return **it;
    }
    
    RecordList::iterator index()
    {
        return *it;
    }
    
    NameIndex::iterator record()
    {
        return it;
    }
    
    IteratorName(Group::Query query = {}) : query(query) {}
    IteratorName(NameIndex::iterator it, Group::Query query = {}) : it(it), query(query)
    {
        while(!end(it))
        {
            if(test(*it, query))
            {
                return *this;
            }
            ++it;
        }
        return *this;
    }
};

class Group::IteratorRecord : Group::Iterator
{
    RecordList::iterator it;
public:
    
    virtual const IteratorRecord &operator++()
    {
        while(!end(it))
        {
            if(test(*it, query))
            {
                return *this;
            }
        }
        return *this;
    }
    
    virtual Record &operator*
    {
        return *it;
    }
    
    RecordList::iterator record()
    {
        return it;
    }
    
    NameIndex::iterator index()
    {
        return names.find(*it);
    }
    
    IteratorRecord(Group::Query query = {}) : query(query) {}
    IteratorRecord(Records::iterator it, Group::Query query = {}) : it(it), query(query) {}
};

#endif
