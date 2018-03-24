#ifndef GROUPS_H
#define GROUPS_H

#include <utility>

#include "dlist.h"
#include "group.h"

class GroupList : private DList<Group>
{
    NameIndex names;
public:
    using DList<Group>::iterator;
    using DList<Group>::erase;
    using DList<Group>::clear;
    using DList<Group>::empty;
    using DList<Group>::begin;
    using DList<Group>::end;

    iterator insert(Group &&group)
    {
        for(auto it = begin(); it != end(); ++it)
        {
            if(*it > group)
                return insert(it, std::move(group));
        }
        return insert(end(), std::move(group));
    }

    iterator insert(Record &&record)
    {
        auto it = lower_bound(record.group);
        if(it != end() && *it == record.group)
        {
            it->insert(std::move(record));
        }
        else
        {
            group = new Group;
            group.insert(record);
            it = insert(it, std::move(group));
        }
        names.insert(it);
        return it;
    }

    iterator find(int key)
    {
        for(auto it = begin(); it != end(); ++it)
        {
            if(*it == key)
                return it;
        }
        return end();
    }

    iterator lower_bound(int key)
    {
        for(auto it = begin(); it != end(); ++it)
        {
            if(*it >= key)
                return it;
        }
        return end();
    }

    iterator upper_bound(int key)
    {
        for(auto it = begin(); it != end(); ++it)
        {
            if(*it > key)
                return it;
        }
        return end();
    }
}

#endif
