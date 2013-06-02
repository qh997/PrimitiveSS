#include "stdio.h"

void kernel_main()
{
    early_printk("kernel start.\n");

    while (1);
}
