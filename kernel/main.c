#include "stdio.h"
#include "sys/sched.h"
#include "sys/system.h"

void restart();

void kernel_main()
{
    early_printk("kernel start.\n");

    sched_init();
    restart();

    INFINITE();
}
