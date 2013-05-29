#include "types.h"

void *memcpy(void *d, void *s, int c)
{
    void *ret = d;
    while (c--)
        *(u8 *)d++ = *(u8 *)s++;

    return ret;
}
