#include "external_index.h"

typename ExternalIndex::iterator ExternalIndex::lower_bound(char *name)
{   
    return lower_bound(Key{name});
}   

typename ExternalIndex::iterator ExternalIndex::upper_bound(char *name)
{   
    return upper_bound(Key{name});
}   
