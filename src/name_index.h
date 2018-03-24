#ifndef NAME_INDEX_H
#define NAME_INDEX_H

#include <algorithm>
#include <cstring>
#include <utility>

#include "avl_tree.h"
#include "dlist.h"
#include "record.h"
#include "records.h"

bool less(RecordList::iterator &a, RecordList::Iterator &b)
{
    auto c = std::strcmp(a->name(), b->name());
    return c < 0;
}

class NameIndex : private AvlTree<RecordList::iterator, less>
{
public:
    void index(RecordList::iterator it)
    {
        auto tmp = it;
        insert(std::move(tmp));
    }


};

#endif
