#ifndef __PROC_H__
#define __PROC_H__

#include "types.h"
#include "sys/sched.h"

#define SEND 1
#define RECV 2
#define BOTH (SEND | RECV)

struct proc_msg {
};

int pmsg_sendrecv(int func, int srcdst, struct proc_msg *msg);
int sys_sendrecv(struct proc *p, int func, int srcdst, struct proc_msg *msg);

#endif
