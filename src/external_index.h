#ifndef EXTERNAL_INDEX
#define EXTERNAL_INDEX

#include "avl_tree.h"
#include "record.h"

#include <cstring>

bool less(const Record * const &r1, const Record * const &r2)
{
    auto cmp = std::strcmp(r1->name(), r2->name());
    return cmp < 0;
}

struct Key
{
    char *str;
};

bool operator<(const Key &key, const Record * const &rec)
{
    return std::strcmp(key.str, rec->name()) < 0;
}

bool operator<(const Record * const &rec, const Key &key)
{
    return std::strcmp(rec->name(), key.str) < 0;
}

class ExternalIndex : private AvlTree<const Record*, less>
{
    using Base = AvlTree<const Record*, less>;
    using Base::lower_bound;
    using Base::upper_bound;

public:
    using Base::iterator;
    using Base::begin;
    using Base::end;
    using Base::insert;

    iterator lower_bound(char *name);
    iterator upper_bound(char *name);
};

#endif
