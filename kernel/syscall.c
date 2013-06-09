#include "sys/interrupts.h"
#include "sys/syscall.h"
#include "sys/protect.h"
#include "sys/sched.h"
#include "sys/proc.h"
#include "stdio.h"

extern void int_syscall();
extern int sys_pmsg_send(struct proc *p, int dest, void *c);
extern int sys_pmsg_receive(struct proc *p, int source, void *c);

int scall_tmp1(struct proc *p)
{
    //early_printk("-scall_tmp1-");
    for (int i = 0; i < 0xfffff; i++);
    p->status = STATUS_RECEIVING;
    schedule();
    return 12;
}

int scall_tmp3(struct proc *p, int a, int b, void *c)
{
    for (int i = 0; i < 0xfffff; i++);
    //early_printk("-scall_tmp3(%x,%x,%x)", a, b, c);

    //early_printk("[c = %x]-", *(int *)c);
    pid2proc(a)->status = STATUS_RUNNING;

    return 0xffd;
}

syscall_handler syscall_table[] = {
    NULL,
    sys_pmsg_send,
    sys_pmsg_receive,
    scall_tmp1,
    scall_tmp3,
};

void syscall_init()
{
    init_idt_desc(INT_SYSCALL, DA_386IGate, int_syscall, PRIVI_USER);
    early_printk("syscall1(%x)\n", (u32)syscall_table[1]);
    early_printk("syscall2(%x)\n", (u32)syscall_table[2]);
}

//_syscall2(int, pmsg_send, 1, int, struct proc_msg *);
//_syscall2(int, pmsg_receive, 2, int, struct proc_msg *);

_syscall0(int, syscall_tmp1, 3);
_syscall3(int, syscall_tmp3, 4, int, int, void *);
