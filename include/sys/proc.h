#ifndef __PROC_H__
#define __PROC_H__

#include "types.h"
#include "sys/sched.h"

#define SEND 1
#define RECV 2
#define BOTH (SEND | RECV)

#define ANY (NR_PROCS + 1)

enum msg_type {
    INVALID = 0,
    RECEIPT,
    INT,
    WRITE,
};

struct proc_msg {
    int type;
    int sender;

    int num;
    void *content;
};

void proc_init();
int pmsg_send(int dest, struct proc_msg *msg);
int pmsg_receive(int source, struct proc_msg *msg);
int send_recv(int func, int src_dst, struct proc_msg *msg);
void inform_int(int pid);

#endif
