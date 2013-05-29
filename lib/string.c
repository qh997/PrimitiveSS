#include "types.h"
#include "sys/global.h"
#include "sys/proto.h"

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

char *strcpy(char *d, char *s)
{
    char *ret = d;
    while ((*d++ = *s++));
    return ret;
}

int strlen(char *d)
{
    int ret = 0;
    while (*d++)
        ret++;
    return ret;
}
