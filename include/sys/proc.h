#ifndef __PROC_H__
#define __PROC_H__

#include "types.h"
#include "sys/sched.h"

#define SEND 1
#define RECV 2
#define BOTH (SEND | RECV)

#define ANY (NR_PROCS + 1)

struct proc_msg {
    int num;
};

int pmsg_send(int dest, struct proc_msg *msg);
int pmsg_receive(int source, struct proc_msg *msg);
int send_recv(int func, int src_dst, struct proc_msg *msg);
void proc_init();

#define proc2linear(pid, addr) \
    ((u32)(vir2lin(pid2proc(pid)->ldt[INDEX_LDT_DATA], addr)))

#endif
