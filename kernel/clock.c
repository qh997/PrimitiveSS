#include "sys/interrupts.h"
#include "sys/proto.h"
#include "stdio.h"
#include "types.h"

void clock_handler_a();

u32 jiffies = 0;

void clock_handler()
{
    early_printk("c");
}

void clock_init()
{
    /* 初始化 8253 PIT */
    out_b(TIMER_MODE, RATE_GENERATOR);
    out_b(TIMER0, (u8)(TIMER_FREQ / HZ));
    out_b(TIMER0, (u8)((TIMER_FREQ / HZ) >> 8));

    /* 设定时钟中断处理程序 */
    register_irq_handler(INT_CLOCK, clock_handler_a);
    enable_irq(INT_CLOCK);
}
