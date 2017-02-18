#ifndef UTIL_H
#define UTIL_H
#include "../include/types.h"
index min(index a, index b)
{
    if (a < b)
    {
        return a;
    }
    return b;
} __attribute__((pure))
#endif
