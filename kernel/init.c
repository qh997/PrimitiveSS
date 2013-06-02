#include "sys/global.h"
#include "sys/proto.h"
#include "string.h"
#include "sys/interrupts.h"
#include "stdio.h"
#include "sys/protect.h"

void init()
{
    disp_str("kernel init ... ");

    protect_init();
    PIC8259A_init();

    early_printk("OK\n");
}
