#include "sys/proc.h"

void task_tty()
{
    struct proc_msg msg;
    send_recv(RECV, ANY, &msg);
}
