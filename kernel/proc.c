#include "sys/proc.h"
#include "sys/syscall.h"
#include "sys/sched.h"
#include "stdio.h"
#include "string.h"

struct msg_queue {
    u8 flag;
    struct proc *from;
    struct proc *to;
    struct proc_msg *msg;
    struct msg_queue *next;
} *msg_head = NULL;

struct msg_queue msg_list[256];
#define MSGLIST_START msg_list[0]
#define MSGLIST_END   msg_list[256]

int sys_pmsg_send(struct proc *p, int d, struct proc_msg *m)
{
    pid2proc(d)->status = STATUS_RUNNING;

    return 0;
}

int sys_pmsg_receive(struct proc *p, int s, struct proc_msg *m)
{
    p->status = STATUS_RECEIVING;
    schedule();

    return 0;
}
