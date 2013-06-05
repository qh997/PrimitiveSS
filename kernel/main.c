#include "stdio.h"
#include "sys/sched.h"

void kernel_main()
{
    early_printk("kernel start.\n");

    sched_init();

    __asm__("pushfl ; andl $0xffffbfff,(%esp) ; popfl");
    ltr(0);
    lldt(0);

    move_to_user_mode();

    early_printk("In user mode now!\n");

    while (1);
}
