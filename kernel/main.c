#include "stdio.h"
#include "sys/sched.h"
#include "sys/system.h"
#include "sys/proc.h"

void restart();

#define STACK_SIZE (500 * 4)

u8 stackA[STACK_SIZE];

void ProcA()
{
    int j = 0;
    while (1) {
        early_printk("A");
        for (int i = 0; i < 0xfffff; i++);
        if (j++ == 10) {
            //pmsg_send(1, NULL);
            send_recv(SEND, 1, NULL);
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
        
        if (j++ == 1) {
            //pmsg_receive(ANY, NULL);
            send_recv(RECV, ANY, NULL);
            early_printk("bbb");
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

    proc_init(ProcA, "Proc A", 1, stackA, 512);
    proc_init(ProcB, "Proc B", 10, stackA, 1024);
    //proc_init(ProcC, "Proc C", 1, stackA, 1500);
    //proc_init(ProcD, "Proc D", 1, stackA, 2000);

    restart();

    INFINITE();
}
