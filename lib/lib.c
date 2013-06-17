#include "types.h"
#include "sys/global.h"
#include "sys/proto.h"
#include "sys/proc.h"
#include "string.h"

int send_recv(int func, int src_dst, struct proc_msg *msg)
{
    int ret = -1;
    if (func == RECV && msg)
        memset(msg, 0x0, sizeof(struct proc_msg));

    switch (func) {
        case SEND:
            ret = pmsg_send(src_dst, msg);
            break;

        case RECV:
            ret = pmsg_receive(src_dst, msg);
            break;

        case BOTH:
            ret = pmsg_send(src_dst, msg);
            if (ret == 0)
                ret = pmsg_receive(src_dst, msg);
            break;

        default:
            break;
    }

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
