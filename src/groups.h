#ifndef GROUPS_H
#define GROUPS_H

#include <utility>

#include "dlist.h"
#include "group.h"

class GroupList : private DList<Group>
{
public:
    using DList<Group>::iterator;
    using DList<Group>::erase;
    using DList<Group>::clear;
    using DList<Group>::empty;
    using DList<Group>::begin;
    using DList<Group>::end;

    iterator insert(Group &&group);
    iterator insert(Record &&record);
    iterator find(int key);
    iterator lower_bound(int key);
    iterator upper_bound(int key);
private:
    using DList<Group>::insert;
};
#endif
