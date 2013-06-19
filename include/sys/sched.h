#ifndef __SCHED_H__
#define __SCHED_H__

#include "sys/protect.h"
#include "sys/system.h"
#include "types.h"

#define TASK_NAME_LEN (32)

typedef void (*p_entry)();

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
    struct desc_seg ldt[NR_LDT];

    int status;
    size_t pid;

    int priority;
    int counter;

    bool income_int;
};

struct task {
    p_entry eip;
    u8 *esp;
    char name[TASK_NAME_LEN];
};

#define ltr() __asm__("ltr %%ax"::"a" (SEL_TSS))
#define set_desc_ldt(n, addr) \
    do { \
        init_desc(&gdt[n + INDEX_1ST_LDT], \
                  vir2lin(gdt[INDEX_DATA], addr), \
                  NR_LDT * sizeof(struct desc_seg) - 1, \
                  DA_LDT \
                  ); \
    } while (0)

extern struct proc proc_table[];
extern struct proc *current;
extern u8 k_reenter;

#define NR_PROCS   PROC_PG_NR
#define FIRST_PROC proc_table[0]
#define LAST_PROC  proc_table[NR_PROCS]

#define DEFAULT_STACK_SIZE (0x4000)

#define STATUS_RUNNING    0
#define STATUS_SENDING    1
#define STATUS_RECEIVING  2
#define STATUS_INVALID   -1

#define TASK_TTY 0

void new_proc(p_entry entry, char *name, int prior, u8 *stk_top);
void sched_init();
void schedule();

#define pid2proc(pid) (&proc_table[pid])
#define proc2pid(proc) (proc - proc_table)
#define proc2linear(pid, addr) \
    ((u32)(vir2lin(pid2proc(pid)->ldt[INDEX_LDT_DATA], addr)))

#endif
