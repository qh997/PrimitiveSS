#ifndef __SCHED_H__
#define __SCHED_H__

#include "sys/protect.h"
#include "sys/system.h"

struct proc {
    struct desc_seg ldt[LDT_SIZE];
    struct tss tss;
};

#define _LDT(n) ((((u32)n) << 4) + SEL_1ST_LDT)
#define _TSS(n) ((((u32)n) << 4) + SEL_1ST_TSS)

#define ltr(n) __asm__("ltr %%ax"::"a" (_TSS(n)))
#define lldt(n) __asm__("lldt %%ax"::"a" (_LDT(n)))

#define set_desc_ldt(n, addr) \
    do { \
        init_desc(&gdt[n + INDEX_1ST_LDT], \
                  vtol(SEL_DATA, addr), \
                  LDT_SIZE * sizeof(struct desc_seg) - 1, \
                  DA_LDT \
                  ); \
    } while (0)
#define set_desc_tss(n, addr) \
    do { \
        init_desc(&gdt[n + INDEX_1ST_TSS], \
                  vtol(SEL_DATA, addr), \
                  sizeof(struct tss) - 1, \
                  DA_386TSS \
                  ); \
    } while (0)

extern struct proc proc_table[];
extern struct proc *current;

void proc_init(int n);
void sched_init();

#endif
