#include "sys/proc.h"
#include "sys/syscall.h"
#include "sys/sched.h"

struct msg_queue {
    u8 flag;
    struct proc *from;
    struct proc *to;
    struct proc_msg *msg;
    struct msg_queue *next;
} *msg_head = NULL;

struct msg_queue msg_list[256];
#define MSGLIST_START msg_list[0]
#define MSGLIST_END   msg_list[256 - 1]

static int msg_send(struct proc *p, int dest, struct proc_msg *msg)
{
    struct msg_queue *mq = &MSGLIST_START;
    for (; mq <= &MSGLIST_END; mq++)
        if (!mq->flag)
            break;

    mq->from = p;
    mq->to = pid2proc(dest);
    mq->msg = msg;

    p->status = STATUS_SENDING;
    schedule();

    return 0;
}

static int msg_receive(struct proc *p, int source, struct proc_msg *msg)
{
    return 0;
}

int sys_sendrecv(struct proc *p, int func, int srcdst, struct proc_msg *msg)
{
    switch (func) {
        case SEND:
            msg_send(p, srcdst, msg);
            break;

        case RECV:
            msg_receive(p, srcdst, msg);
            break;

        default:
            break;
    }

    return 0;
}
