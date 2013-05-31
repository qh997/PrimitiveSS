#include "sys/protect.h"
#include "sys/interrupts.h"
#include "sys/global.h"
#include "types.h"
#include "stdio.h"

char *exception_msg[] = {
    "#DE Divide Error",
    "#DB RESERVED",
    "--  NMI Interrupt",
    "#BP Breakpoint",
    "#OF Overflow",
    "#BR BOUND Range Exceeded",
    "#UD Invalid Opcode (Undefined Opcode)",
    "#NM Device Not Available (No Math Coprocessor)",
    "#DF Double Fault",
    "    Coprocessor Segment Overrun (reserved)",
    "#TS Invalid TSS",
    "#NP Segment Not Present",
    "#SS Stack-Segment Fault",
    "#GP General Protection",
    "#PF Page Fault",
    "--  (Intel reserved. Do not use.)",
    "#MF x87 FPU Floating-Point Error (Math Fault)",
    "#AC Alignment Check",
    "#MC Machine Check",
    "#XF SIMD Floating-Point Exception"
};

static void init_idt_desc(unsigned char vector, u8 desc_type, int_handler handler, unsigned char privilege);

#define EXCEPTION_HANDLER(vector, name, code) \
    void name(int t, ...) { \
        va_list arg = (va_list)((int *)&t - 1); \
        if (!code) { \
            disp_str("Exception! --> "); \
            disp_str(exception_msg[vector]); \
            early_printk( \
                "\n\nEFLAGS:%x CS:%x EIP:%x\n", \
                *((int *)arg + 2), \
                *((int *)arg + 1), \
                *((int *)arg + 0) \
            ); \
        } \
        else { \
            disp_str("Exception! --> "); \
            disp_str(exception_msg[vector]); \
            early_printk( \
                "\n\nEFLAGS: %x CS: %x EIP: %x ERR: %x\n", \
                *((int *)arg + 3), \
                *((int *)arg + 2), \
                *((int *)arg + 1), \
                *((int *)arg + 0) \
            ); \
        }\
        while (1) ; \
    }

EXCEPTION_HANDLER(INT_VECTOR_DIVIDE,       divide_error,          FALSE);
EXCEPTION_HANDLER(INT_VECTOR_DEBUG,        single_step_exception, FALSE);
EXCEPTION_HANDLER(INT_VECTOR_NMI,          nmi,                   FALSE);
EXCEPTION_HANDLER(INT_VECTOR_BREAKPOINT,   breakpoint_exception,  FALSE);
EXCEPTION_HANDLER(INT_VECTOR_OVERFLOW,     overflow,              FALSE);
EXCEPTION_HANDLER(INT_VECTOR_BOUNDS,       bounds_check,          FALSE);
EXCEPTION_HANDLER(INT_VECTOR_INVAL_OP,     inval_opcode,          FALSE);
EXCEPTION_HANDLER(INT_VECTOR_COPROC_NOT,   copr_not_available,    FALSE);
EXCEPTION_HANDLER(INT_VECTOR_DOUBLE_FAULT, double_fault,          TRUE );
EXCEPTION_HANDLER(INT_VECTOR_COPROC_SEG,   copr_seg_overrun,      FALSE);
EXCEPTION_HANDLER(INT_VECTOR_INVAL_TSS,    inval_tss,             TRUE );
EXCEPTION_HANDLER(INT_VECTOR_SEG_NOT,      segment_not_present,   TRUE );
EXCEPTION_HANDLER(INT_VECTOR_STACK_FAULT,  stack_exception,       TRUE );
EXCEPTION_HANDLER(INT_VECTOR_PROTECTION,   general_protection,    TRUE );
EXCEPTION_HANDLER(INT_VECTOR_PAGE_FAULT,   page_fault,            TRUE );
EXCEPTION_HANDLER(INT_VECTOR_COPROC_ERR,   copr_error,            FALSE);

void init_protect()
{
    init_idt_desc(INT_VECTOR_DIVIDE,       DA_386IGate, divide_error,          PRIVI_KRNL);
    init_idt_desc(INT_VECTOR_DEBUG,        DA_386IGate, single_step_exception, PRIVI_KRNL);
    init_idt_desc(INT_VECTOR_NMI,          DA_386IGate, nmi,                   PRIVI_KRNL);
    init_idt_desc(INT_VECTOR_BREAKPOINT,   DA_386IGate, breakpoint_exception,  PRIVI_USER);
    init_idt_desc(INT_VECTOR_OVERFLOW,     DA_386IGate, overflow,              PRIVI_USER);
    init_idt_desc(INT_VECTOR_BOUNDS,       DA_386IGate, bounds_check,          PRIVI_KRNL);
    init_idt_desc(INT_VECTOR_INVAL_OP,     DA_386IGate, inval_opcode,          PRIVI_KRNL);
    init_idt_desc(INT_VECTOR_COPROC_NOT,   DA_386IGate, copr_not_available,    PRIVI_KRNL);
    init_idt_desc(INT_VECTOR_DOUBLE_FAULT, DA_386IGate, double_fault,          PRIVI_KRNL);
    init_idt_desc(INT_VECTOR_COPROC_SEG,   DA_386IGate, copr_seg_overrun,      PRIVI_KRNL);
    init_idt_desc(INT_VECTOR_INVAL_TSS,    DA_386IGate, inval_tss,             PRIVI_KRNL);
    init_idt_desc(INT_VECTOR_SEG_NOT,      DA_386IGate, segment_not_present,   PRIVI_KRNL);
    init_idt_desc(INT_VECTOR_STACK_FAULT,  DA_386IGate, stack_exception,       PRIVI_KRNL);
    init_idt_desc(INT_VECTOR_PROTECTION,   DA_386IGate, general_protection,    PRIVI_KRNL);
    init_idt_desc(INT_VECTOR_PAGE_FAULT,   DA_386IGate, page_fault,            PRIVI_KRNL);
    init_idt_desc(INT_VECTOR_COPROC_ERR,   DA_386IGate, copr_error,            PRIVI_KRNL);
}

void init_desc(struct desc_seg *p_desc, u32 base, u32 limit, u16 attribute)
{
    p_desc->limit_0        = limit & 0xFFFF;
    p_desc->base_0         = base & 0xFFFF;
    p_desc->base_1         = (base >> 16) & 0xFF;
    p_desc->type_0         = attribute & 0xFF;
    p_desc->limit_1_type_1 = ((limit>>16) & 0x0F) | ((attribute>>8) & 0xF0);
    p_desc->base_2         = (base >> 24) & 0xFF;
}

static void init_idt_desc(unsigned char vector, u8 desc_type, int_handler handler, unsigned char privilege)
{
    struct desc_gate *p_gate = &idt[vector];
    u32 base = (u32)handler;

    p_gate->offset_0 = base & 0xFFFF;
    p_gate->selector = SEL_T;
    p_gate->pcount   = 0;
    p_gate->attr     = desc_type | (privilege << 5);
    p_gate->offset_1 = (base >> 16) & 0xFFFF;
}
