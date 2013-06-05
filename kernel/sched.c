#include "sys/sched.h"
#include "sys/protect.h"
#include "stdio.h"
#include "string.h"

#define STACK_SIZE (1024)

struct tss   tss;
struct proc proc_table[PROC_PG_NR];
struct proc *current;
u8 k_reenter;

u8 stackA[STACK_SIZE];
u8 stackB[STACK_SIZE];

void ProcA()
{
    while (1) {
        early_printk("A");
        for (int i = 0; i < 0xfffff; i++);
    }
}

void ProcB()
{
    while (1) {
        early_printk("B");
        for (int i = 0; i < 0xfffff; i++);
    }
}

void proc_init(pentry entry, char *name, u8 *s)
{
    int i = 0;
    for (i = 0; i < PROC_PG_NR; i++)
        if (!proc_table[i].used)
            break;

    struct proc *p = (struct proc *)proc_table + i;
    memset(p, 0x0, sizeof(struct proc));

    p->used = TRUE;

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
    p->regs.esp = (u32)((u8 *)s + STACK_SIZE);
    p->regs.eflags = 0x3202;

    p->priority = PROC_PG_NR / (i + 1);
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

    memset(&proc_table, 0x0, PROC_PG_NR * sizeof(struct proc));
    proc_init(ProcA, "Proc A", stackA);
    proc_init(ProcB, "Proc B", stackB);

    k_reenter = 0;
    current = proc_table;
}

void schedule()
{
    int c = -1;
    while (TRUE) {
        for (int i = 0; i < PROC_PG_NR; i++) {
            struct proc *p = proc_table + i;
            if (!p->used)
                continue;
            if (p->counter > c)
                c = p->counter, current = p;
        }

        if (c) break;
        for (int i = 0; i < PROC_PG_NR; i++) {
            struct proc *p = proc_table + i;
            if (p->used)
                p->counter = p->priority;
        }
    }
}
