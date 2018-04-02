#include "external_index.h"

bool less(Record * const &r1, Record * const &r2)
{
    auto cmp = std::strcmp(r1->name(), r2->name());
    return cmp < 0;
}

bool operator<(const Key &key, const Record * const &rec)
{
    return std::strcmp(key.str, rec->name()) < 0;
}

bool operator<(const Record * const &rec, const Key &key)
{
    return std::strcmp(rec->name(), key.str) < 0;
}

bool operator==(const Record * const &ptr, const Record &rec)
{
    return *ptr == rec;
}
bool operator==(const Record &rec, const Record * const &ptr)
{
    return ptr == rec;
}
bool operator<(const Record *const &ptr, const Record &rec)
{
    return std::strcmp(ptr->name(), rec.name()) < 0;
}
bool operator<(const Record &rec, const Record *const &ptr)
{
    return std::strcmp(rec.name(), ptr->name()) < 0;
}

typename ExternalIndex::iterator ExternalIndex::lower_bound(char *name)
{   
    return lower_bound(Key{name});
}   

typename ExternalIndex::iterator ExternalIndex::upper_bound(char *name)
{   
    return upper_bound(Key{name});
}   
