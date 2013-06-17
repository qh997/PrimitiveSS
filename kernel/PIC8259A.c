#include "sys/proto.h"
#include "sys/interrupts.h"
#include "string.h"

hw_irq_handler hwirq_table[HWIRQ_NR];

void PIC8259A_init()
{
    memset(hwirq_table, 0x0, sizeof(hw_irq_handler) * HWIRQ_NR);

    out_b(INT_M_CTL, 0x11);          // 主8259, ICW1
    out_b(INT_S_CTL, 0x11);          // 从8259, ICW1
    out_b(INT_M_CTLMASK, INT_PIC_M); // 主8259, ICW2
    out_b(INT_S_CTLMASK, INT_PIC_S); // 从8259, ICW2
    out_b(INT_M_CTLMASK, 0x04);      // 主8259, ICW3
    out_b(INT_S_CTLMASK, 0x02);      // 从8259, ICW3
    out_b(INT_M_CTLMASK, 0x01);      // 主8259, ICW4
    out_b(INT_S_CTLMASK, 0x01);      // 从8259, ICW4
    out_b(INT_M_CTLMASK, 0xFF);      // 主8259, OCW1
    out_b(INT_S_CTLMASK, 0xFF);      // 从8259, OCW1

    clock_init();
}

void register_hwirq_handler(int irq, hw_irq_handler handler)
{
    disable_hwirq(irq);
    hwirq_table[irq] = handler;
}
