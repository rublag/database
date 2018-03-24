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

    cursor remove(const DatabaseCursor cursor)
    {

    }
};

class SearchIterator
{
    ???iterator it;
    DatabaseQuery &query;

public:
    SearchIterator(DatabaseQuery &q) : query(q);

    const SearhIterator &operator++()
    {
        while(!end(it))
        {
            ++it;
            if(test(*it, query))
            {
                return it;
            }
        }
        return it;
    }

    Record &operator*()
    {
        return *it;
    }        
};

#endif
