#include "sys/interrupts.h"
#include "sys/syscall.h"
#include "sys/protect.h"
#include "sys/sched.h"
#include "stdio.h"

extern void int_syscall();

int scall_tmp1(struct proc *p)
{
    early_printk("-scall_tmp1-");
    return 12;
}

int scall_tmp3(struct proc *p, int a, int b, void *c)
{
    early_printk("-scall_tmp3(%x,%x,%x)", a, b, c);

    early_printk("[c = %x]-", *(int *)c);

    return 0xffd;
}

syscall_handler syscall_table[] = {
    scall_tmp1,
    scall_tmp3,
};

void syscall_init()
{
    init_idt_desc(INT_SYSCALL, DA_386IGate, int_syscall, PRIVI_USER);
}

_syscall0(int, syscall_tmp1, 0);
_syscall3(int, syscall_tmp3, 1, int, int, void *);
