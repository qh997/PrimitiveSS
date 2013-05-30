#include "sys/protect.h"
#include "sys/interrupts.h"
#include "sys/global.h"
#include "types.h"
#include "stdio.h"

void exception(int, int);
static void init_idt_desc(unsigned char vector, u8 desc_type, int_handler handler, unsigned char privilege);

#define INT_HANDLER(vector, name, code) \
    void name() { if (!code) exception(vector, 0xFFFFFFFF); }

INT_HANDLER(INT_VECTOR_DIVIDE,       divide_error,          FALSE);
INT_HANDLER(INT_VECTOR_DEBUG,        single_step_exception, FALSE);
INT_HANDLER(INT_VECTOR_NMI,          nmi,                   FALSE);
INT_HANDLER(INT_VECTOR_BREAKPOINT,   breakpoint_exception,  FALSE);
INT_HANDLER(INT_VECTOR_OVERFLOW,     overflow,              FALSE);
INT_HANDLER(INT_VECTOR_BOUNDS,       bounds_check,          FALSE);
INT_HANDLER(INT_VECTOR_INVAL_OP,     inval_opcode,          FALSE);
INT_HANDLER(INT_VECTOR_COPROC_NOT,   copr_not_available,    FALSE);
INT_HANDLER(INT_VECTOR_DOUBLE_FAULT, double_fault,          TRUE );
INT_HANDLER(INT_VECTOR_COPROC_SEG,   copr_seg_overrun,      FALSE);
INT_HANDLER(INT_VECTOR_INVAL_TSS,    inval_tss,             TRUE );
INT_HANDLER(INT_VECTOR_SEG_NOT,      segment_not_present,   TRUE );
INT_HANDLER(INT_VECTOR_STACK_FAULT,  stack_exception,       TRUE );
INT_HANDLER(INT_VECTOR_PROTECTION,   general_protection,    TRUE );
INT_HANDLER(INT_VECTOR_PAGE_FAULT,   page_fault,            TRUE );
INT_HANDLER(INT_VECTOR_COPROC_ERR,   copr_error,            FALSE);

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

void exception_handler(int vec_no, int err_code, int eip, int cs, int eflags)
{
    char *err_msg[] = {
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

    disp_int((int)err_msg[0]);
    disp_str("\n\n");
    disp_str("Exception! --> ");
    disp_str(err_msg[vec_no]);
    disp_str("\n\n");
    disp_str("EFLAGS:");
    disp_int(eflags);
    disp_str("CS:");
    disp_int(cs);
    disp_str("EIP:");
    disp_int(eip);

    if (err_code != 0xFFFFFFFF) {
        disp_str("Error code:");
        disp_int(err_code);
    }
    while (1);
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
