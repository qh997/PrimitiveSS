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
    struct proc_msg msg;
    msg.num = 999;
    early_printk("[A]");
    int j = 0;
    while (1) {
        //early_printk("A");
        for (int i = 0; i < 0xfffff; i++) ;
        if (j++ == 30) {
            msg.num = j;
            early_printk("1");
            send_recv(SEND, 0, &msg);
            early_printk("a");
        }
    }
}

void ProcB()
{
    struct proc_msg msg;
    early_printk("[B]");
    int j = 0;
    while (1) {
        //early_printk("B");
        for (int i = 0; i < 0xfffff; i++);

        if (j++ == 1) {
            early_printk("2");
            send_recv(RECV, ANY, &msg);
            early_printk("b(%d)", msg.num);
        }
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
    proc_init();

    new_proc(ProcB, "Proc B", 15, stackB + STACK_SIZE);
    new_proc(ProcA, "Proc A", 15, stackA + STACK_SIZE);
    //new_proc(ProcC, "Proc C", 2, stackC, STACK_SIZE);
    //new_proc(ProcD, "Proc D", 10, stackD, STACK_SIZE);

    restart();

    INFINITE();
}
