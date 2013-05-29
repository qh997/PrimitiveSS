#ifndef __STRING_H__
#define __STRING_H__

#include "types.h"

void *memcpy(void *d, void *s, size_t n);
void *memset(void *d, u8 ch, size_t n);
char *strcpy(char *d, char *s);
int strlen(char *d);

#endif
