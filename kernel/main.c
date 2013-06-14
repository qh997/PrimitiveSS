#include "stdio.h"
#include "sys/sched.h"
#include "sys/system.h"
#include "sys/proc.h"

extern int disp_pos;
void restart();

#define STACK_SIZE (0x4000)

u8 stackA[STACK_SIZE];
u8 stackB[STACK_SIZE];
u8 stackC[STACK_SIZE];
u8 stackD[STACK_SIZE];

void ProcA()
{
    int j = 0;
    while (1) {
        early_printk("A");
        for (int i = 0; i < 0xfffff; i++) ;
        //early_printk("(A%d)", j);
        if (j++ == 12) {
            early_printk("1");
            send_recv(SEND, 1, NULL);
            early_printk("a");
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
        if (j == 4) {
            early_printk("2");
            send_recv(RECV, ANY, NULL);
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

    proc_init(ProcB, "Proc B", 20, stackB, STACK_SIZE);
    proc_init(ProcA, "Proc A", 4, stackA, STACK_SIZE);
    proc_init(ProcC, "Proc C", 2, stackC, STACK_SIZE);
    proc_init(ProcD, "Proc D", 10, stackD, STACK_SIZE);

    restart();

    INFINITE();
}
