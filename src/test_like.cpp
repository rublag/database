#include "test_like.h"

static bool match_underscore(char *hs, char *n)
{
    if(*n == '\0')
        return false;
    return test_like(hs+1, n+1);
}

static bool match_percent(char *hs, char *n)
{
    do
    {
        if(test_like(hs, n))
            return true;
    } while(*hs++ != '\0');
    return false;
}

bool test_like(char *hs, char *n)
{
    if(!hs || !n)
        return false;

    if(*n == '\0' && *hs == '\0')
        return true;
    else if(*n == '\0')
        return false;
    if(*n == '%')
    {
        return match_percent(hs, n+1);
    }
    else if(*n == '_')
    {
        return match_underscore(hs, n+1);
    }
    else if(*hs == *n)
    {
        return test_like(hs+1, n+1);
    }
    return false;
}
