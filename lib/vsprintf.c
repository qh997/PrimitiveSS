#include "types.h"
#include "string.h"
#include "stdio.h"

char *uitoa(char *buf, unsigned int val, int base)
{
    char *p = buf;
    int rem = val % base;
    int quo = val / base;

    if (0 == val) {
        *p++ = '0';
        *p++ = 0;
        return buf;
    }

    char tmp[64];
    char *t = tmp;
    while (rem || quo) {
        char dig = rem > 9 ? 'A' + rem - 10 : '0' + rem;

        *t++ = dig;
        rem = quo % base;
        quo = quo / base;
    }

    while (t-- > tmp)
        *p++ = *t;
    *p = 0;

    return buf;
}

int vsprintf(char *buf, const char *fmt, va_list args)
{
    char *p;
    char tmp[STR_DEFAULT_LEN];
    va_list p_next_arg = args;

    for (p = buf; *fmt; fmt++) {
        if (*fmt != '%') {
            *p++ = *fmt;
            continue;
        }

        fmt++;

        switch (*fmt) {
            case 'x':
                uitoa(tmp, *((int *)p_next_arg), 16);
                strcpy(p, tmp);
                p_next_arg += 4;
                p += strlen(tmp);
                break;
            case 's':
                break;
            default:
                break;
        }
    }

    *p = 0;

    return (p - buf);
}

int sprintf(char *buf, const char *fmt, ...)
{
    va_list arg = (va_list)((char *)(&fmt) + 4);
    return vsprintf(buf, fmt, arg);
}
