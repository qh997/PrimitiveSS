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
        printk("[A]");
        for (int i = 0; i < 0xfffff; i++);
    }
}

void ProcB()
{
    while (1) {
        printk("[B]");
        for (int i = 0; i < 0xfffff; i++);
    }
}

void ProcC()
{
    while (1) {
        printk("[C]");
        for (int i = 0; i < 0xfffff; i++);
    }
}

void ProcD()
{
    while (1) {
        printk("[D]");
        for (int i = 0; i < 0xfffff; i++);
    }
}

void kernel_main()
{
    early_printk("kernel start.\n");

    sched_init();
    proc_init();

    creat_proc(ProcA, "Proc A", 10, stackA + STACK_SIZE);
    creat_proc(ProcB, "Proc B", 20, stackB + STACK_SIZE);
    creat_proc(ProcC, "Proc C", 15, stackC + STACK_SIZE);
    creat_proc(ProcD, "Proc D", 30, stackD + STACK_SIZE);

    restart();

    INFINITE();
}
