#ifndef __SCHED_H__
#define __SCHED_H__

#include "sys/protect.h"
#include "sys/system.h"

typedef void (*pentry)();

struct strackframe {
    u32 gs;         // ┓
    u32 fs;         // ┃
    u32 es;         // ┣ save 压栈
    u32 ds;         // ┛
    u32 edi;        // ┓
    u32 esi;        // ┃
    u32 ebp;        // ┃
    u32 kernel_esp; // ┃
    u32 ebx;        // ┣ pushad 压栈
    u32 edx;        // ┃
    u32 ecx;        // ┃
    u32 eax;        // ┛
    u32 retaddr;    // call save 时压栈
    u32 eip;        // ┓
    u32 cs;         // ┃
    u32 eflags;     // ┣ 中断发生时压栈
    u32 esp;        // ┃
    u32 ss;         // ┛
};

struct proc {
    struct strackframe regs;
    u16 sel_ldt;
    struct desc_seg ldt[LDT_SIZE];

    int priority;
    int counter;

    u8 used;
};

#define ltr() __asm__("ltr %%ax"::"a" (SEL_TSS))
#define set_desc_ldt(n, addr) \
    do { \
        init_desc(&gdt[n + INDEX_1ST_LDT], \
                  vtol(SEL_DATA, addr), \
                  LDT_SIZE * sizeof(struct desc_seg) - 1, \
                  DA_LDT \
                  ); \
    } while (0)

extern struct proc proc_table[];
extern struct proc *current;
extern u8 k_reenter;

void proc_init(pentry entry, char *name, u8 *s);
void sched_init();
void schedule();

#endif
