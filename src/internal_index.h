#include <cstring>

#include "avl_tree.h"
#include "record.h"
#include "records.h"

bool less(const typename RecordList::iterator &i1, const typename RecordList::iterator &i2)
{
    return std::strcmp(i1->name(), i2->name()) < 0;
}

bool operator<(char * const &str, const typename RecordList::iterator &it)
{
    return std::strcmp(str, it->name()) < 0;
}

bool operator<(const typename RecordList::iterator &it, char * const &str)
{
    return std::strcmp(it->name(), str) < 0;
}

bool operator==(const typename RecordList::iterator & it, const Record &rec)
{
    return rec == *it;
}

bool operator==(const Record &rec, const typename RecordList::iterator &it)
{
    return it == rec;
}

bool operator<(const typename RecordList::iterator & it, const Record &rec)
{
    return it < rec.name();
}

bool operator<(const Record &rec, const typename RecordList::iterator &it)
{
    return rec.name() < it;
}

class InternalIndex : AvlTree<RecordList::iterator, less>
{
    using Base = AvlTree<RecordList::iterator, less>;

public:
    using Base::iterator;
    using Base::begin;
    using Base::end;
    using Base::insert;
    using Base::lower_bound;
    using Base::upper_bound;
    using Base::erase;
    using Base::find;

    iterator lower_bound(char *name);
    iterator upper_bound(char *name);

};
