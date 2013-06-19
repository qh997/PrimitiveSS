#include "sys/proc.h"
#include "sys/sched.h"

u8 tty_stack[DEFAULT_STACK_SIZE];

void task_tty()
{
    early_printk("tty task\n");
    struct proc_msg msg;
    send_recv(RECV, ANY, &msg);

    while (1);
}
