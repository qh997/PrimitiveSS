#include "sys/global.h"
#include "sys/proto.h"
#include "string.h"
#include "sys/interrupts.h"
#include "stdio.h"
#include "sys/protect.h"

void init()
{
    disp_str("kernel init ... ");

    memset(gdt, 0x0, NR_GDT * sizeof(struct desc_seg));
    memcpy((void *)&gdt, (void *)(*(u32 *)&gdtr[2]), *(u16 *)(&gdtr[0]) + 1);
    *(u16 *)(&gdtr[0]) = NR_GDT * sizeof(struct desc_seg) - 1;
    *(u32 *)(&gdtr[2]) = (u32)&gdt;

    memset(idt, 0x0, NR_IDT * sizeof(struct desc_gate));
    *(u16 *)(&idtr[0]) = NR_IDT * sizeof(struct desc_gate) - 1;
    *(u32 *)(&idtr[2]) = (u32)&idt;

    init_interrupts();
    init_protect();

    disp_str("OK\n");
}
