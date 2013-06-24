#include "stdio.h"
#include "sys/sched.h"
#include "sys/system.h"
#include "sys/proc.h"

extern int disp_pos;
void restart();

#define STACK_SIZE (0x8000)

u8 stackA[STACK_SIZE];
u8 stackB[STACK_SIZE];
u8 stackC[STACK_SIZE];
u8 stackD[STACK_SIZE];

void ProcA()
{
    while (1) {
        //early_printk("A");
        printk("[A]");
        for (int i = 0; i < 0xfffff; i++);
    }
}

void ProcB()
{
    while (1) {
        //early_printk("B");
        printk("[B]");
        for (int i = 0; i < 0xfffff; i++);
    }
}

void ProcC()
{
    while (1) {
        //early_printk("C");
        printk("[C]");
        for (int i = 0; i < 0xfffff; i++);
    }
}

void ProcD()
{
    while (1) {
        //early_printk("D");
        printk("[D]");
        for (int i = 0; i < 0xfffff; i++);
    }
}

void kernel_main()
{
    early_printk("kernel start.\n");

    sched_init();
    proc_init();

    new_proc(ProcA, "Proc A", 30, stackA + STACK_SIZE);
    new_proc(ProcB, "Proc B", 30, stackB + STACK_SIZE);
    new_proc(ProcC, "Proc C", 30, stackC + STACK_SIZE);
    new_proc(ProcD, "Proc D", 30, stackD + STACK_SIZE);

    restart();

    INFINITE();
}
