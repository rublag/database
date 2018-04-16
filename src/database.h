#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <fstream>

#include "record.h"
#include "groups.h"
#include "external_index.h"

class Database
{
    public:
    class Query;
    private:
    GroupList groups;
    ExternalIndex  names;

    bool test(const Record &record, const Query &query) const;
    static Group::Query makeGroupQuery(Query &q);
public:
    class Iterator;
    using iterator = Iterator;
    class SearchIterator;
    void insert(Record &&record);
    void remove(const Query &query);
    iterator select(Query query);
    iterator erase(iterator it);
    void erase(const Record &record);

    Database(std::istream &istr);
    Database() = default;
};

struct Database::Query
{
    enum class Operator {
        Nil = 0, Eq, Ne, Gt, Ge, Lt, Le, Like
    };

    Operator nameOp, groupOp, phoneOp;
    char *name;
    int group;
    int phone;
};

class Database::Iterator
{
    friend class Database;
    Query query;
    ExternalIndex::iterator names_iterator;
    GroupList::iterator groups_iterator;
    Group::iterator group_iterator;
    enum class Uses {
        Nil = 0, Name, Groups
    } uses;
    bool end = false;

public:
    const Iterator &operator++();
    Record &operator*();

    Iterator(ExternalIndex::iterator lower, Query q);
    Iterator(GroupList::iterator lower, Query q);
    bool atEnd();

private:
    void satisfyPredicate();
    void getFirstMatch();
    bool match(const Record &record) const;
};
#endif
