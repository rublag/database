#ifndef STORAGE_H
#define STORAGE_H

#include "record.h"

struct DatabaseQuery
{
    enum class Operator {
        Nil = 0, Eq, Ne, Gt, Ge, Lt, Le
    };

    Operator nameOp, groupOp, phoneOp;
    char *name;
    int group;
    int phone;
};

class DatabaseCursor
{
public:
    bool next();
    Record &get();
    bool end();
};

class Database
{
    GroupList records;
    NameIndex  names;

    bool test(const Record &record, const DatabaseQuery &query) const
    {
        switch(query.nameOp)
        {
        case DatabaseQuery::Operator::Nil:
            break;
        case DatabaseQuery::Operator::Eq:
            if(std::strcmp(query.name, record.name()) != 0)
                return false;
            break;
        case DatabaseQuery::Operator::Ne:
            if(std::strcmp(query.name, record.name()) == 0)
                return false;
            break;
        case DatabaseQuery::Operator::Lt:
            if(std::strcmp(query.name, record.name()) >= 0)
                return false;
            break;
        case DatabaseQuery::Operator::Le:
            if(std::strcmp(query.name, record.name()) > 0)
                return false;
            break;
        case DatabaseQuery::Operator::Gt:
            if(std::strcmp(query.name, record.name()) <= 0)
                return false;
            break;
        case DatabaseQuery::Operator::Ge:
            if(std::strcmp(query.name, record.name()) < 0)
                return false;
            break;
        }

        switch(query.groupOp)
        {
        case DatabaseQuery::Operator::Nil:
            break;
        case DatabaseQuery::Operator::Eq:
            if(query.group != record.group)
                return false;
            break;
        case DatabaseQuery::Operator::Ne:
            if(query.group == record.group)
                return false;
            break;
        case DatabaseQuery::Operator::Lt:
            if(query.group >= record.group)
                return false;
            break;
        case DatabaseQuery::Operator::Le:
            if(query.group > record.group)
                return false;
            break;
        case DatabaseQuery::Operator::Gt:
            if(query.group <= record.group)
                return false;
            break;
        case DatabaseQuery::Operator::Ge:
            if(query.group < record.group)
                return false;
            break;
        }
        
        switch(query.phoneOp)
        {
        case DatabaseQuery::Operator::Nil:
            break;
        case DatabaseQuery::Operator::Eq:
            if(query.phone != record.phone)
                return false;
            break;
        case DatabaseQuery::Operator::Ne:
            if(query.phone == record.phone)
                return false;
            break;
        case DatabaseQuery::Operator::Lt:
            if(query.phone >= record.phone)
                return false;
            break;
        case DatabaseQuery::Operator::Le:
            if(query.phone > record.phone)
                return false;
            break;
        case DatabaseQuery::Operator::Gt:
            if(query.phone <= record.phone)
                return false;
            break;
        case DatabaseQuery::Operator::Ge:
            if(query.phone < record.phone)
                return false;
            break;
        }
        return true;
    }
    
    Group::Query makeGroupQuery(Query &q)
    {
        auto gq = Group::Query {};
        gq.name = q.name;
        gq.phone = q.phone;
        
        switch(query.name())
        {
            case Query::Operator::Eq:
                gq.nameOp = Group::Query::Operator::Eq;
                break;
            case Query::Operator::Ne:
                gq.nameOp = Group::Query::Operator::Ne;
                break;
            case Query::Operator::Lt:
                gq.nameOp = Group::Query::Operator::Lt;
                break;
            case Query::Operator::Le:
                gq.nameOp = Group::Query::Operator::Le;
                break;
            case Query::Operator::Gt:
                gq.nameOp = Group::Query::Operator::Gt;
                break;
            case Query::Operator::Ge:
                gq.nameOp = Group::Query::Operator::Ge;
                break;
            case Query::Operator::Nil:
                gq.nameOp = Group::Query::Operator::Nil;
                break;
        }
        
        switch(query.phone())
        {
            case Query::Operator::Eq:
                gq.phoneOp = Group::Query::Operator::Eq;
                break;
            case Query::Operator::Ne:
                gq.phoneOp = Group::Query::Operator::Ne;
                break;
            case Query::Operator::Lt:
                gq.phoneOp = Group::Query::Operator::Lt;
                break;
            case Query::Operator::Le:
                gq.phoneOp = Group::Query::Operator::Le;
                break;
            case Query::Operator::Gt:
                gq.phoneOp = Group::Query::Operator::Gt;
                break;
            case Query::Operator::Ge:
                gq.phoneOp = Group::Query::Operator::Ge;
                break;
            case Query::Operator::Nil:
                gq.phoneOp = Group::Query::Operator::Nil;
                break;
        }
    }
public:
    class SearchIterator;
    query(const DatabaseQuery &query)
    {
        if(query.groupOp)
        {
            switch(query.groupOp)
        }
    }
    void insert(Record &&record)
    {
        auto it = groups.find(record.group);
        if(it == groups.end())
        {
            auto group = new Group(record.group);
            it = groups.insert(group);
        }

        auto git = it->insert(group);
        names.index(git);
    }

    void remove(Query query)
    {
        if(!query.groupOp && query.nameOp)
        {
            ;
        }
        if(!query.groupOp)
        {
            auto it = groups.begin();
            auto gq = makeGroupQuery(query);
            while(it != groups.end())
            {
                it->remove(query);
                ++it;
            }
        }
    }
    
    Iterator select(Query query)
    {
        if(query.groupOp)
        {
            auto gq = makeGroupQuery(query);
            auto it = groups.begin();
            while(it != groups.end())
            {
                auto it = group->select(gq);
                while(!it.end())
                {
                    
                }
            }
        }
    }
};

class Database::Iterator
{
public:
    const Iterator &operator++() =0;
    Record &operator*() =0;
    
    NameIndex index()
};

class Database::IteratorGroup : Database::Iterator
{
    Groups::iterator it;
    Group::iterator gi;
    Group::Query gq;
    Database::Query;
   
public:
    virtual const Database::IteratorGroup &operator++()
    {
        while(it != groups.end())
        {
            while(!gi.end())
            {
                ++gi;
                if(test(gi))
                    return *this;
            }
            ++it;
            auto gq = makeGroupQuery(query);
            gi = it->select(
            if(test(it))
                return *this;
        }
        return *this;
    }
    
    virtual Group::Iterator;
}

#endif
