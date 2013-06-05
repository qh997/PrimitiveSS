#include "sys/protect.h"
#include "sys/interrupts.h"
#include "sys/global.h"
#include "types.h"
#include "stdio.h"
#include "string.h"

struct desc_seg gdt[NR_GDT];
struct desc_gate idt[NR_IDT];
u8 gdtr[6];
u8 idtr[6];

static void init_idt_desc(unsigned char vector, u8 desc_type, void *handler, unsigned char privilege)
{
    struct desc_gate *p_gate = &idt[vector];
    u32 base = (u32)handler;

    p_gate->offset_0 = base & 0xFFFF;
    p_gate->selector = SEL_TEXT;
    p_gate->pcount   = 0;
    p_gate->attr     = desc_type | (privilege << 5);
    p_gate->offset_1 = (base >> 16) & 0xFFFF;
}

static void ignore_int()
{
    early_printk("Unknown interrupt\n");
}

void register_irq_handler(int vector, void *handler)
{
    struct desc_gate *p_gate = &idt[vector];
    u32 base = (u32)handler;

    p_gate->offset_0 = base & 0xFFFF;
    p_gate->offset_1 = (base >> 16) & 0xFFFF;
}

void init_desc(struct desc_seg *desc, u32 base, u32 limit, u16 type)
{
    desc->limit_0        = limit & 0xFFFF;
    desc->base_0         = base & 0xFFFF;
    desc->base_1         = (base >> 16) & 0xFF;
    desc->type_0         = type & 0xFF;
    desc->limit_1_type_1 = ((limit>>16) & 0x0F) | ((type>>8) & 0xF0);
    desc->base_2         = (base >> 24) & 0xFF;
}

void protect_init()
{
    memset(gdt, 0x0, NR_GDT * sizeof(struct desc_seg));
    memcpy((void *)&gdt, (void *)(*(u32 *)&gdtr[2]), *(u16 *)(&gdtr[0]) + 1);
    *(u16 *)(&gdtr[0]) = NR_GDT * sizeof(struct desc_seg) - 1;
    *(u32 *)(&gdtr[2]) = (u32)&gdt;

    memset(idt, 0x0, NR_IDT * sizeof(struct desc_gate));
    *(u16 *)(&idtr[0]) = NR_IDT * sizeof(struct desc_gate) - 1;
    *(u32 *)(&idtr[2]) = (u32)&idt;

    for (int i = 0; i < NR_IDT; i++)
        init_idt_desc(i, DA_386IGate, ignore_int, PRIVI_KRNL);

    for (int i = 3; i < 6; i++)
        idt[i].attr &= 0x9f | (PRIVI_USER << 5);
}
