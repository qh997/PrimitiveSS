#include "types.h"
#include "sys/global.h"
#include "sys/proto.h"
#include "sys/proc.h"
#include "string.h"

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
