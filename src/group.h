#ifndef GROUP_H
#define GROUP_H


#include "internal_index.h"
#include "record.h"
#include "records.h"

class Group
{
public:
    struct Query;
    class Iterator;
private:
    int _group = 0;
    InternalIndex  names;
    RecordList records;
public:
    using iterator = Iterator;
    int group() const;
    Record &insert(Record &&record);
    iterator select(Query query) const;
    iterator erase(iterator s);
    void erase(const Record &record);

    Group(int group);
    Group(Group&&);
};

struct Group::Query
{
    enum class Operator {
        Nil = 0, Eq, Ne, Gt, Ge, Lt, Le, Like
    } nameOp, phoneOp;
    char *name;
    int  phone;
};

class Group::Iterator
{
    friend class Group;
    Query query;
    InternalIndex::iterator names_iterator;
    RecordList::iterator records_iterator;
    enum class Uses {
        Nil = 0, Name, Records
    } uses;
    bool end = false;

public:
    const Iterator &operator++();
    Record &operator*();
    bool atEnd();

    Iterator(InternalIndex::iterator lower, Query q);
    Iterator(RecordList::iterator lower, Query q);
    Iterator();

private:
    void getFirstMatch();
    void satisfyPredicate();
    bool match(const Record &record, int *name_res = nullptr) const;
};

bool operator==(const Group &g, int i);
bool operator==(const Group &g1, const Group &g2);
bool operator==(int i, const Group &g);
#endif
