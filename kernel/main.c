#include "stdio.h"
#include "sys/sched.h"
#include "sys/system.h"
#include "sys/proc.h"

void restart();

#define STACK_SIZE (1024)

u8 stackA[STACK_SIZE];
u8 stackB[STACK_SIZE];
//u8 stackC[STACK_SIZE];
//u8 stackD[STACK_SIZE];

void ProcA()
{
    int j = 0;
    while (1) {
        early_printk("A");
        for (int i = 0; i < 0xfffff; i++);
        //early_printk("(A%d)", j);
        if (j++ == 6) {
            //send_recv(SEND, 1, NULL);
        }
    }
}

void ProcB()
{
    early_printk("B");
    int j = 0;
    while (1) {
        early_printk("B");
        for (int i = 0; i < 0xfffff; i++);
        
        //early_printk("(B%d)", j);
        if (j == 3) {
            //send_recv(RECV, ANY, NULL);
            early_printk("b");
        }
        j++;
    }
}

void ProcC()
{
    while (1) {
        early_printk("C");
        for (int i = 0; i < 0xfffff; i++);
    }
}

void ProcD()
{
    while (1) {
        early_printk("D");
        for (int i = 0; i < 0xfffff; i++);
    }
}

void kernel_main()
{
    early_printk("kernel start.\n");

    sched_init();

    proc_init(ProcA, "Proc A", 400, stackA, STACK_SIZE);
    proc_init(ProcB, "Proc B", 500, stackB, STACK_SIZE);
    //proc_init(ProcC, "Proc C", 2, stackC, STACK_SIZE - 128 * 2);
    //proc_init(ProcD, "Proc D", 3, stackA, STACK_SIZE - 128 * 3);

    restart();

    INFINITE();
}
