#include "internal_index.h"
#include "record.h"

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


typename InternalIndex::iterator InternalIndex::lower_bound(char *name)
{   
    return Base::lower_bound(name);
}   

typename InternalIndex::iterator InternalIndex::upper_bound(char *name)
{   
    return Base::upper_bound(name);
}

/*typename InternalIndex::iterator InternalIndex::find(const Record &record)
{   
    return Base::lower_bound(record);
}
*/
