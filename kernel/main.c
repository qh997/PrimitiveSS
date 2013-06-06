#include "stdio.h"
#include "sys/sched.h"
#include "sys/system.h"

void restart();

#define STACK_SIZE (1024)

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

void kernel_main()
{
    early_printk("kernel start.\n");

    sched_init();

    proc_init(ProcA, "Proc A", 15, stackA, STACK_SIZE);
    proc_init(ProcB, "Proc B", 45, stackB, STACK_SIZE);

    restart();

    INFINITE();
}
