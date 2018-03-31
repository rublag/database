#include "group.h"
#include "record.h"
#include "records.h"
#include "name_index.h"

const Group::IteratorRecord &Group::IteratorRecord::operator++()
{
    while(1)//!end(it))
    {
        if(1)//(test(*it, query))
        {
            return *this;
        }
    }
    return *this;
}

Record &Group::IteratorRecord::operator*()
{
    return *it;
}

RecordList::iterator Group::IteratorRecord::record()
{
    return it;
}

NameIndex::iterator Group::IteratorRecord::index()
{
    //            return names.find(*it);
}

Group::IteratorRecord::IteratorRecord(Group::Query q)
{
    query = q;
}

Group::IteratorRecord::IteratorRecord(RecordList::iterator it, Group::Query q) : it(it)
{
    query = q;
}
