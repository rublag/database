#include "internal_index.h"
#include "record.h"

typename InternalIndex::iterator InternalIndex::lower_bound(char *name)
{   
    return Base::lower_bound(name);
}   

typename InternalIndex::iterator InternalIndex::upper_bound(char *name)
{   
    return Base::upper_bound(name);
}

typename InternalIndex::iterator InternalIndex::find(const Record &record)
{   
    return Base::lower_bound(record);
}
