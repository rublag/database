#include "groups.h"
#include <utility>

#include "dlist.h"
#include "group.h"

typename GroupList::iterator GroupList::insert(Group &&group)
{
    for(auto it = begin(); it != end(); ++it)
    {
        if(it->group() > group.group())
            return insert(it, std::move(group));
    }
    return insert(end(), std::move(group));
}

typename GroupList::iterator GroupList::insert(Record &&record)
{
    auto it = lower_bound(record.group());
    if(it != end() && it->group() == record.group())
    {
        it->insert(std::move(record));
    }
    else
    {
        Group group(record.group());
        group.insert(std::move(record));
        it = insert(it, std::move(group));
    }
    return it;
}

typename GroupList::iterator GroupList::find(int key)
{
    for(auto it = begin(); it != end(); ++it)
    {
        if(*it == key)
            return it;
    }
    return end();
}

typename GroupList::iterator GroupList::lower_bound(int key)
{
    for(auto it = begin(); it != end(); ++it)
    {
        if(it->group() >= key)
            return it;
    }
    return end();
}

typename GroupList::iterator GroupList::upper_bound(int key)
{
    for(auto it = begin(); it != end(); ++it)
    {
        if(it->group() > key)
            return it;
    }
    return end();
}
