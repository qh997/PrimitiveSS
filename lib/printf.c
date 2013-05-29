#include "types.h"
#include "sys/global.h"
#include "sys/proto.h"
#include "types.h"
#include "stdio.h"

void disp_str(char *s)
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

int early_printk(char *fmt, ...)
{
    int i;
    char buf[STR_DEFAULT_LEN];

    va_list arg = (va_list)((char *)(&fmt) + 4);
    i = vsprintf(buf, fmt, arg);
    disp_str(buf);

    return i;
}
