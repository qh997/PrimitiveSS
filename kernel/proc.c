#include "sys/proc.h"
#include "sys/syscall.h"
#include "sys/sched.h"
#include "stdio.h"

struct msg_queue {
    u8 flag;
    struct proc *from;
    struct proc *to;
    struct proc_msg *msg;
    //struct msg_queue *next;
} *msg_head = NULL;

struct msg_queue msg_list[256];
#define MSGLIST_START msg_list[0]
#define MSGLIST_END   msg_list[256]

int sys_pmsg_send(struct proc *p, int dest, void *c)
{
    early_printk("*sys_pmsg_send*");
    pid2proc(dest)->status = STATUS_RUNNING;

    return 0;
}

int sys_pmsg_receive(struct proc *p, int source, void *c)
{
    early_printk("*%x sys_pmsg_receive %x*", k_reenter, p - &FIRST_PROC);
    p->status = STATUS_RECEIVING;
    schedule();

    return 0;
}

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
