#include "test_like.h"

bool test_like(char *hs, char *n)
{
    if(*hs == '%')
    {
        return test_like(hs+1, n) || test_like(hs, n+1);
    }
    else if(*hs == '_')
    {
        return test_like(hs+1, n+1);
    }
    else
    {
        return *hs == *n;
    }
}
