#include "types.h"
#include "sys/global.h"
#include "sys/proto.h"
#include "types.h"
#include "stdio.h"
#include "string.h"
#include "sys/system.h"
#include "sys/proc.h"

void disp_str(char *s)
{
    for (; *s; s++) {
        u8 *pos = (u8 *)vir2lin(gdt[INDEX_VIDO], disp_pos);
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

void disp_int(int n)
{
    char buf[STR_DEFAULT_LEN];
    uitoa(buf, n, 16);
    disp_str(buf);
}

int early_printk(const char *fmt, ...)
{
    int i;
    char buf[STR_DEFAULT_LEN];
    va_list arg = (va_list)((void **)&fmt + 1);
    i = vsprintf(buf, fmt, arg);
    disp_str(buf);

    return i;
}

int printk(const char *fmt, ...)
{
    int i;
    char buf[STR_DEFAULT_LEN];
    va_list arg = (va_list)((void **)&fmt + 1);
    i = vsprintf(buf, fmt, arg);

    struct proc_msg msg;
    msg.type = WRITE;
    msg.num = 0;
    msg.content = buf;

    send_recv(SEND, TASK_TTY, &msg);

    return i;
}
