#include "sys/proc.h"
#include "sys/syscall.h"
#include "sys/sched.h"
#include "stdio.h"
#include "string.h"
#include "types.h"

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
#define MSGLIST_1ST   (msg_list)
#define MSGLIST_LAST  (msg_list + 256)

#define MQ_INVA 0
#define MQ_SEND 1
#define MQ_RECV 2

void proc_init()
{
    memset(&msg_list, 0x0, 256 * sizeof(struct msg_queue));
    early_printk("msg_list(%x, %x)\n", msg_list, sizeof(struct msg_queue));
    msg_head = MSGLIST_LAST - 1;
}

static struct msg_queue *get_recver_msg(struct proc *p)
{
    struct msg_queue *mq;
    for (mq = MSGLIST_1ST; mq < MSGLIST_LAST; mq++)
        if (mq->flag == MQ_RECV && mq->to == proc2pid(p))
            break;

    if (mq == MSGLIST_LAST)
        return NULL;
    else
        return mq;
}

static struct msg_queue *get_sender_msg(struct proc *p)
{
    struct msg_queue *mq;
    for (mq = MSGLIST_1ST; mq < MSGLIST_LAST; mq++)
        if (mq->flag == MQ_SEND && mq->to == proc2pid(p))
            break;

    if (mq == MSGLIST_LAST)
        return NULL;
    else
        return mq;
}

static int msg_queue_insert(u8 f, int fm, int to, struct proc_msg *m)
{
    struct msg_queue *msg_head_old = msg_head;
    do {
        msg_head++;
        if (msg_head == MSGLIST_LAST)
            msg_head = MSGLIST_1ST;
    } while (msg_head_old != msg_head && msg_head->flag != MQ_INVA);

    if (msg_head_old == msg_head)
        return -1;

    memset(msg_head, 0, sizeof(struct msg_queue));

    msg_head->flag = f;
    msg_head->from = fm;
    msg_head->to = to;
    msg_head->msg = m;

    return 0;
}

static inline void free_mq(struct msg_queue *mq)
{
    mq->flag = MQ_INVA;
}

static void deliver_msg(struct msg_queue *d, struct msg_queue *s)
{
    memcpy(
        (void *)proc2linear(d->to, d->msg),
        (void *)proc2linear(s->from, s->msg),
        sizeof(struct proc_msg)
    );

    free_mq(d);
    free_mq(s);
}

int sys_pmsg_send(struct proc *p, int d, struct proc_msg *m)
{
    struct proc *pd = pid2proc(d);
    struct msg_queue *tod = get_recver_msg(pd);
    ((struct proc_msg *)proc2linear(proc2pid(p), m))->sender = proc2pid(p);

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
        p->status = STATUS_SENDING;
        schedule();
    }

    return 0;
}

int sys_pmsg_receive(struct proc *p, int s, struct proc_msg *m)
{
    struct msg_queue *tome = get_sender_msg(p);
    if (tome) {
        struct msg_queue dst = {
            flag:MQ_RECV,
            from:s,
            to:proc2pid(p),
            msg:m,
            next:NULL,
        };
        deliver_msg(&dst, tome);
        pid2proc(tome->from)->status = STATUS_RUNNING;
    }
    else {
        msg_queue_insert(MQ_RECV, s, proc2pid(p), m);
        p->status = STATUS_RECEIVING;
        schedule();
    }

    return 0;
}

void inform_int(int pid)
{
    struct proc *p = pid2proc(pid);
    if (p->status == STATUS_RECEIVING) {
        struct msg_queue *mq = get_recver_msg(p);
        if (mq) {
            mq->msg->type = INT;
            p->status = STATUS_RUNNING;
            mq->flag = MQ_INVA;
        }
    }
    else {
        p->income_int = TRUE;
    }
}
