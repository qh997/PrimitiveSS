#include "sys/sched.h"
#include "sys/protect.h"
#include "stdio.h"
#include "string.h"

struct tss   tss;
struct proc proc_table[NR_PROCS];
struct proc *current;
u8 k_reenter;

void schedule()
{
    int c = -1;
    struct proc *p;
    int old = current - &FIRST_PROC;

    while (TRUE) {
        for (p = &FIRST_PROC; p < &LAST_PROC; p++)
            if ((p->status == STATUS_RUNNING) && (p->counter > c)) {
                c = p->counter;
                current = p;
            }

        if (c) break;
        for (p = &FIRST_PROC; p <= &LAST_PROC; p++)
            if (p->status == STATUS_RUNNING)
                p->counter = p->priority;
    }
    int new = current - &FIRST_PROC;
    if (old != new) {
        //early_printk("[%x->%x]", old, new);
    }
}

void proc_init(p_entry entry, char *name, int prior, u8 *stk, size_t stk_size)
{
    int i = 0;
    for (i = 0; i < NR_PROCS; i++)
        if (proc_table[i].status == STATUS_INVALID)
            break;

    early_printk("{%s}-pid(%d)", name, i);
    struct proc *p = (struct proc *)proc_table + i;
    memset(p, 0x0, sizeof(struct proc));

    p->ldt[INDEX_LDT_TEXT] = gdt[INDEX_TEXT];
    p->ldt[INDEX_LDT_TEXT].type_0 = DA_C | PRIVI_USER << 5;
    p->ldt[INDEX_LDT_DATA] = gdt[INDEX_DATA];
    p->ldt[INDEX_LDT_DATA].type_0 = DA_DRW | PRIVI_USER << 5;
    p->sel_ldt = SEL_1ST_LDT + (i << 3);

    set_desc_ldt(i, &proc_table[i].ldt);
    early_printk("-ldt(%x)", gdt[INDEX_1ST_LDT + i]);

    p->regs.cs = SEL_LDT_TEXT | SA_TIL | PRIVI_USER;
    p->regs.ds =
    p->regs.es =
    p->regs.fs =
    p->regs.ss = SEL_LDT_DATA | SA_TIL | PRIVI_USER;
    p->regs.gs = (SEL_VIDO & SA_RPL_MASK) | PRIVI_USER;
    early_printk("-cs(%x)-ss(%x)", p->regs.cs, p->regs.ss);

    p->regs.eip = (u32)entry;
    p->regs.esp = (u32)(stk + stk_size);
    p->regs.eflags = 0x3202;
    early_printk("-esp(%x)", p->regs.esp);

    p->status = STATUS_RUNNING;
    p->priority = prior;
    p->pid = i;
    early_printk("\n");
}

void sched_init()
{
    memset(&tss, 0x0, sizeof(struct tss));
    tss.ss0 = SEL_DATA;
    tss.iobase = sizeof(struct tss);
    init_desc(
        &gdt[INDEX_TSS],
        vtol(SEL_DATA, &tss),
        sizeof(struct tss) - 1,
        DA_386TSS
    );
    ltr();

    memset(&proc_table, 0x0, NR_PROCS * sizeof(struct proc));
    for (struct proc *p = &FIRST_PROC; p < &LAST_PROC; p++)
        p->status = STATUS_INVALID;

    k_reenter = 0;
    current = proc_table;
}
