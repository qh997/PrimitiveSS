#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#define INT_M_CTL     0x20
#define INT_M_CTLMASK 0x21
#define INT_S_CTL     0xA0
#define INT_S_CTLMASK 0xA1

/* 中断向量 */
#define TRAP_DIVIDE       0x0
#define TRAP_DEBUG        0x1
#define TRAP_NMI          0x2
#define TRAP_BREAKPOINT   0x3
#define TRAP_OVERFLOW     0x4
#define TRAP_BOUNDS       0x5
#define TRAP_INVAL_OP     0x6
#define TRAP_COPROC_NOT   0x7
#define TRAP_DOUBLE_FAULT 0x8
#define TRAP_COPROC_SEG   0x9
#define TRAP_INVAL_TSS    0xA
#define TRAP_SEG_NOT      0xB
#define TRAP_STACK_FAULT  0xC
#define TRAP_PROTECTION   0xD
#define TRAP_PAGE_FAULT   0xE
#define TRAP_COPROC_ERR   0x10

#define INT_VECTOR_IRQ0         0x20
#define INT_VECTOR_IRQ8         0x28

#define NR_IRQS 16

typedef void (*irq_handler)(int irq);
//typedef void (*excp_handler)(int b, int c, int d);

extern irq_handler irq_table[];

extern void PIC8259A_init();
void trap_init();

extern void disable_irq(int irq);
extern void enable_irq(int irq);
extern void register_irq_handler(int vector, void *handler);

#endif
