#include "types.h"
#include "sys/global.h"
#include "sys/proto.h"

void early_printk(char *s)
{
    for (; *s; s++) {
        u8 *pos = (u8 *)(V_MEM_BASE + disp_pos);
        if (*s == '\n') {
            disp_pos = (disp_pos / 160 + 1) * 160;
            continue;
        }
        *pos++ = *s;
        *pos++ = 0x7;
        disp_pos += 2;
    }
    update_cursor();
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

void out_b(u16 port, u8 val)
{
    __asm__(
        "out %%al, %%dx\n\t"
        "nop\n\tnop"
        :
        :"a"(val),"d"(port)
    );
}

u8 in_b(u16 port)
{
    u8 val;
    __asm__(
        "in %%dx, %%al\n\t"
        "nop\n\tnop"
        :"=a"(val)
        :"d"(port)
    );

    return val;
}
