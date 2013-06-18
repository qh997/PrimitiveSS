#include "sys/proc.h"
#include "sys/syscall.h"
#include "sys/sched.h"
#include "stdio.h"
#include "string.h"

struct msg_queue {
    u8 flag;
    int from;
    int to;
    struct proc_msg *msg;
    struct msg_queue *next;
} *msg_head = NULL;

struct msg_queue msg_list[256];
#define MSGLIST_START msg_list[0]
#define MSGLIST_END   msg_list[255]

#define MQ_INVA 0
#define MQ_SEND 1
#define MQ_RECV 2

static struct msg_queue *get_msg_to(struct proc *p)
{
    struct msg_queue *mq;
    for (mq = &MSGLIST_END; mq > &MSGLIST_START; mq--)
        if (mq->flag != MQ_INVA && mq->to == proc2pid(p))
            break;

    if (mq == &MSGLIST_START)
        return NULL;
    else
        return mq;
}

static int msg_queue_insert(u8 f, int fm, int to, struct proc_msg *m)
{
    struct msg_queue *mq;
    for (mq = &MSGLIST_END; mq > &MSGLIST_START; mq--)
        if (mq->flag == MQ_INVA)
            break;

    if (mq == &MSGLIST_START)
        return -1;

    mq->flag = f;
    mq->from = fm;
    mq->to = to;
    mq->msg = m;

    return 0;
}

static void deliver_msg(struct msg_queue *d, struct msg_queue *s)
{
    memcpy(
        (void *)proc2linear(d->to, d->msg),
        (void *)proc2linear(s->from, s->msg),
        sizeof(struct proc_msg)
    );
}

int sys_pmsg_send(struct proc *p, int d, struct proc_msg *m)
{
    struct proc *pd = pid2proc(d);
    struct msg_queue *tod = get_msg_to(pd);

    if (tod) {
        struct msg_queue src = {
            flag:MQ_SEND,
            from:proc2pid(p),
            to:d,
            msg:m,
            next:NULL,
        };
        deliver_msg(tod, &src);
        pd->status = STATUS_RUNNING;
    }
    else {
        msg_queue_insert(MQ_SEND, proc2pid(p), d, m);
    }

    return 0;
}

int sys_pmsg_receive(struct proc *p, int s, struct proc_msg *m)
{
    struct msg_queue *tome = get_msg_to(p);
    if (tome) {
        struct msg_queue dst = {
            flag:MQ_RECV,
            from:s,
            to:proc2pid(p),
            msg:m,
            next:NULL,
        };
        deliver_msg(&dst, tome);
    }
    else {
        msg_queue_insert(MQ_RECV, s, proc2pid(p), m);
        p->status = STATUS_RECEIVING;
        schedule();
    }

    return 0;
}

void proc_init()
{
    memset(&msg_list, 0x0, 256 * sizeof(struct msg_queue));
}