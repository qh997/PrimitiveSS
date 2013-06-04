#include "sys/sched.h"
#include "sys/protect.h"
#include "stdio.h"
#include "string.h"

struct proc proc_table[PROC_PG_NR];
struct proc *current;

u8 stack[1024];

void ProcA()
{
    early_printk("...ProcA...\n");
    while (1);
}

void proc_init(int n)
{
    struct proc *p = (struct proc *)proc_table + n;
    memset(p, 0x0, sizeof(struct proc));
    p->tss.backlink = 0;
    p->tss.esp0 = (u32)stack + 1024;
    p->tss.ss0 = SEL_DATA;
    p->tss.esp1 = 0;
    p->tss.ss1 = 0;
    p->tss.esp2 = 0;
    p->tss.ss2 = 0;
    p->tss.cr3 = 0;
    p->tss.eip = 0;
    p->tss.eflags = 0;
    p->tss.eax = 0;
    p->tss.ecx = 0;
    p->tss.edx = 0;
    p->tss.ebx = 0;
    p->tss.esp = 0;
    p->tss.ebp = 0;
    p->tss.esi = 0;
    p->tss.edi = 0;
    p->tss.es = 0;
    p->tss.cs = 0;
    p->tss.ss = 0;
    p->tss.ds = 0;
    p->tss.fs = 0;
    p->tss.gs = 0;
    p->tss.ldt = _LDT(0);
    p->tss.trap = 0;
    p->tss.iobase = sizeof(struct tss);

    p->ldt[0] = gdt[INDEX_TEXT];
    p->ldt[0].type_0 = DA_C | PRIVI_USER << 5;
    p->ldt[1] = gdt[INDEX_DATA];
    p->ldt[1].type_0 = DA_C | PRIVI_USER << 5;

    set_desc_ldt(n, &proc_table[n].ldt);
    set_desc_tss(n, &proc_table[n].tss);

    __asm__("pushfl ; andl $0xffffbfff,(%esp) ; popfl");
    ltr(0);
    lldt(0);
}

void sched_init()
{
    proc_init(0);
}
