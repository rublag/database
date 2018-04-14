#ifndef INTERNAL_INDEX_H
#define INTERNAL_INDEX_H

#include <cstring>

#include "avl_tree.h"
#include "record.h"
#include "records.h"

bool less(const typename RecordList::iterator &i1, const typename RecordList::iterator &i2);
bool operator<(char * const &str, const typename RecordList::iterator &it);
bool operator<(const typename RecordList::iterator &it, char * const &str);
bool operator==(const typename RecordList::iterator & it, const Record &rec);
bool operator==(const Record &rec, const typename RecordList::iterator &it);
bool operator<(const typename RecordList::iterator & it, const Record &rec);
bool operator<(const Record &rec, const typename RecordList::iterator &it);

class InternalIndex : AvlTree<RecordList::iterator, less>
{
    using Base = AvlTree<RecordList::iterator, less>;

public:
    using Base::iterator;
    using Base::invariant;
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

#endif
