#include "types.h"

void early_printf(char *s)
{
}

void *memcpy(void *d, void *s, size_t n)
{
    void *ret = d;
    while (n--)
        *(u8 *)d++ = *(u8 *)s++;

    return ret;
}

void *memset(void *d, u8 ch, size_t n)
{
    void *ret = d;
    while (n--)
        *(u8 *)d++ = ch;

    return ret;
}
