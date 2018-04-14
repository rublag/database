#ifndef EXTERNAL_INDEX
#define EXTERNAL_INDEX

#include "avl_tree.h"
#include "record.h"

#include <cstring>

bool less(Record * const &r1, Record * const &r2);
struct Key
{
    char *str;
};
bool operator<(const Key &key, const Record * const &rec);
bool operator<(const Record * const &rec, const Key &key);

bool operator==(const Record * const &ptr, const Record &rec);
bool operator==(const Record &rec, const Record * const &ptr);
bool operator<(const Record *const &ptr, const Record &rec);
bool operator<(const Record &rec, const Record *const &ptr);

class ExternalIndex : private AvlTree<Record*, less>
{
    using Base = AvlTree<Record*, less>;
    using Base::lower_bound;
    using Base::upper_bound;

public:
    using Base::iterator;
    using Base::invariant;
    using Base::begin;
    using Base::end;
    using Base::insert;
    using Base::find;
    using Base::erase;

    iterator lower_bound(char *name);
    iterator upper_bound(char *name);
};

#endif
