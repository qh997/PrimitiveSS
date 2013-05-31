#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#define INT_M_CTL     0x20
#define INT_M_CTLMASK 0x21
#define INT_S_CTL     0xA0
#define INT_S_CTLMASK 0xA1

/* 中断向量 */
#define EXCP_DIVIDE       0x0
#define EXCP_DEBUG        0x1
#define EXCP_NMI          0x2
#define EXCP_BREAKPOINT   0x3
#define EXCP_OVERFLOW     0x4
#define EXCP_BOUNDS       0x5
#define EXCP_INVAL_OP     0x6
#define EXCP_COPROC_NOT   0x7
#define EXCP_DOUBLE_FAULT 0x8
#define EXCP_COPROC_SEG   0x9
#define EXCP_INVAL_TSS    0xA
#define EXCP_SEG_NOT      0xB
#define EXCP_STACK_FAULT  0xC
#define EXCP_PROTECTION   0xD
#define EXCP_PAGE_FAULT   0xE
#define EXCP_COPROC_ERR   0x10

#define INT_VECTOR_IRQ0         0x20
#define INT_VECTOR_IRQ8         0x28

#define NR_IRQS 16

typedef void (*irq_handler)(int irq);
//typedef void (*excp_handler)(int b, int c, int d);

extern irq_handler irq_table[];

void init_interrupts();
void disable_irq(int irq);
void enable_irq(int irq);
void register_irq_handler(int irq, irq_handler handler);
void register_excp_handler(int excp, void *handler);

#endif
