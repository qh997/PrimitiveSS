#include "sys/interrupts.h"
#include "sys/proto.h"
#include "stdio.h"
#include "types.h"
#include "sys/sched.h"

u32 jiffies = 0;

extern void hwint00(int irq);

void clock_handler()
{
    jiffies++;

    if (k_reenter)
        return;

    current->counter--;

    schedule();
}

void clock_init()
{
    /* 初始化 8253 PIT */
    out_b(TIMER_MODE, RATE_GENERATOR);
    out_b(TIMER0, (u8)(TIMER_FREQ / HZ));
    out_b(TIMER0, (u8)((TIMER_FREQ / HZ) >> 8));

    /* 设定时钟中断处理程序 */
    register_irq_handler(INT_CLOCK, hwint00);
    register_hwirq_handler(HWIRQ_CLOCK, clock_handler);
    enable_hwirq(INT_CLOCK);
}
