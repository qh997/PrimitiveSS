#include "sys/proto.h"
#include "sys/interrupts.h"

irq_handler irq_table[NR_IRQS];

void init_8259A()
{
    out_b(INT_M_CTL, 0x11);                // 主8259, ICW1
    out_b(INT_S_CTL, 0x11);                // 从8259, ICW1
    out_b(INT_M_CTLMASK, INT_VECTOR_IRQ0); // 主8259, ICW2
    out_b(INT_S_CTLMASK, INT_VECTOR_IRQ8); // 从8259, ICW2
    out_b(INT_M_CTLMASK, 0x04);            // 主8259, ICW3
    out_b(INT_S_CTLMASK, 0x02);            // 从8259, ICW3
    out_b(INT_M_CTLMASK, 0x01);            // 主8259, ICW4
    out_b(INT_S_CTLMASK, 0x01);            // 从8259, ICW4
    out_b(INT_M_CTLMASK, 0xFF);            // 主8259, OCW1
    out_b(INT_S_CTLMASK, 0xFF);            // 从8259, OCW1
}

void register_irq_handler(int irq, irq_handler handler)
{
    disable_irq(irq);
    irq_table[irq] = handler;
}