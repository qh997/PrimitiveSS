#include "stdio.h"
#include "sys/sched.h"

void restart();

void kernel_main()
{
    early_printk("kernel start.\n");

    sched_init();

    restart();

    while (1);
}
