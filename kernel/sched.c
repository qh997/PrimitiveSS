#include "sys/sched.h"
#include "sys/protect.h"
#include "stdio.h"
#include "string.h"

u8 k_reenter;
struct proc proc_table[NR_PROCS];
struct proc *current;

void schedule()
{
    int c = 0;
    struct proc *p;

    struct proc *old = current;

    while (TRUE) {
        for (p = &FIRST_PROC; p < &LAST_PROC; p++)
            if ((p->status == STATUS_RUNNING) && (p->counter > c)) {
                c = p->counter;
                current = p;
            }

        if (c) break;
        for (p = &FIRST_PROC; p < &LAST_PROC; p++)
            if (p->status == STATUS_RUNNING)
                p->counter = p->priority;
    }

    if (0 && old != current) {
        early_printk("[%d %x", old - &FIRST_PROC, old->regs.eip);
        early_printk(" %d %x %x]", current - &FIRST_PROC, current->regs.eip, current->regs.gs);
    }
}

void new_proc(p_entry entry, char *name, int prior, u8 *stk_top)
{
    int i = 0;
    for (i = 0; i < NR_PROCS; i++)
        if (proc_table[i].status == STATUS_INVALID)
            break;

    struct proc *p = (struct proc *)proc_table + i;
    memset(p, 0x0, sizeof(struct proc));

    p->ldt[INDEX_LDT_TEXT] = gdt[INDEX_TEXT];
    p->ldt[INDEX_LDT_TEXT].type_0 = DA_C | PRIVI_USER << 5;
    p->ldt[INDEX_LDT_DATA] = gdt[INDEX_DATA];
    p->ldt[INDEX_LDT_DATA].type_0 = DA_DRW | PRIVI_USER << 5;
    p->sel_ldt = SEL_1ST_LDT + (i << 3);

    set_desc_ldt(i, &proc_table[i].ldt);

    p->regs.cs = SEL_LDT_TEXT | SA_TIL | PRIVI_USER;
    p->regs.ds =
    p->regs.es =
    p->regs.fs =
    p->regs.ss = SEL_LDT_DATA | SA_TIL | PRIVI_USER;
    p->regs.gs = (SEL_VIDO & SA_RPL_MASK) | PRIVI_USER;

    p->regs.eip = (u32)entry;
    p->regs.esp = (u32)(stk_top);
    p->regs.eflags = 0x3202;

    p->status = STATUS_RUNNING;
    p->priority = prior;
    p->pid = i;

    early_printk("%s (%d){p:%x eip:%x esp:%x gs:%x}\n",
                 name, i, (u32)p, (u32)p->regs.eip,
                 (u32)p->regs.esp, (u32)p->regs.gs);
}

struct tss tss;

void task_tty();
extern u8 tty_stack[];
#define TASK_NR 1
struct task task_table[TASK_NR] = {
    {task_tty, tty_stack + DEFAULT_STACK_SIZE, "tty"},
};

void sched_init()
{
    memset(&tss, 0x0, sizeof(struct tss));
    tss.ss0 = SEL_DATA;
    tss.iobase = sizeof(struct tss);
    init_desc(
        &gdt[INDEX_TSS],
        vir2lin(gdt[INDEX_DATA], &tss),
        sizeof(struct tss) - 1,
        DA_386TSS
    );
    ltr();

    memset(&proc_table, 0x0, NR_PROCS * sizeof(struct proc));
    for (struct proc *p = &FIRST_PROC; p < &LAST_PROC; p++)
        p->status = STATUS_INVALID;

    for (struct task *t = task_table; t < task_table + TASK_NR; t++)
        new_proc(t->eip, t->name, 50, t->esp);

    k_reenter = 0;
    current = proc_table;
}
