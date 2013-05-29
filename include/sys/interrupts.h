#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#define INT_M_CTL     0x20
#define INT_M_CTLMASK 0x21
#define INT_S_CTL     0xA0
#define INT_S_CTLMASK 0xA1

#define INT_VECTOR_IRQ0 0x20
#define INT_VECTOR_IRQ8 0x28

#define NR_IRQS 16

typedef void (*irq_handler)(int irq);

extern irq_handler irq_table[];

void init_8259A();
void disable_irq(int irq);
void enable_irq(int irq);

#endif
