#include "group.h"


const Group::IteratorName &Group::IteratorName::operator++()
{
    while(1)
    {
        ++it;
        if(1)//test(*it, query))
        {
            return *this;
        }
    }
    return *this;
}

Record &Group::IteratorName::operator*()
{
    return **it;
}

NameIndex::iterator Group::IteratorName::index()
{
    return it;
}

RecordList::iterator Group::IteratorName::record()
{
    return *it;
}

Group::IteratorName::IteratorName(Group::Query q) 
{
    query = q;
}

Group::IteratorName::IteratorName(NameIndex::iterator it, Group::Query q) : it(it)
{
    query = q;
    /*while(1)//!end(it))
      {
      if(1)//test(*it, query))
      {
      return *this;
      }
      ++it;
      }
      return *this;*/
}
