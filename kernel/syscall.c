#include "sys/interrupts.h"
#include "sys/syscall.h"
#include "sys/protect.h"
#include "sys/sched.h"
#include "sys/proc.h"
#include "stdio.h"

extern void int_syscall();
extern int sys_pmsg_send(struct proc *p, int d, struct proc_msg *m);
extern int sys_pmsg_receive(struct proc *p, int s, struct proc_msg *m);

syscall_handler syscall_table[] = {
    NULL,
    sys_pmsg_send,
    sys_pmsg_receive,
};

void syscall_init()
{
    set_idt_desc(INT_SYSCALL, DA_386IGate, int_syscall, PRIVI_USER);
}

_syscall2(int, pmsg_send, 1, int, struct proc_msg *);
_syscall2(int, pmsg_receive, 2, int, struct proc_msg *);
