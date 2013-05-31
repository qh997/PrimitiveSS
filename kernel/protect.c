#include "sys/protect.h"
#include "sys/interrupts.h"
#include "sys/global.h"
#include "types.h"
#include "stdio.h"

#define MSG_EXECP_DIVIDE       "#DE Divide Error"
#define MSG_EXECP_DEBUG        "#DB RESERVED"
#define MSG_EXECP_NMI          "--  NMI Interrupt"
#define MSG_EXECP_BREAKPOINT   "#BP Breakpoint"
#define MSG_EXECP_OVERFLOW     "#OF Overflow"
#define MSG_EXECP_BOUNDS       "#BR BOUND Range Exceeded"
#define MSG_EXECP_INVAL_OP     "#UD Invalid Opcode (Undefined Opcode)"
#define MSG_EXECP_COPROC_NOT   "#NM Device Not Available (No Math Coprocessor)"
#define MSG_EXECP_DOUBLE_FAULT "#DF Double Fault"
#define MSG_EXECP_COPROC_SEG   "    Coprocessor Segment Overrun (reserved)"
#define MSG_EXECP_INVAL_TSS    "#TS Invalid TSS"
#define MSG_EXECP_SEG_NOT      "#NP Segment Not Present"
#define MSG_EXECP_STACK_FAULT  "#SS Stack-Segment Fault"
#define MSG_EXECP_PROTECTION   "#GP General Protection"
#define MSG_EXECP_PAGE_FAULT   "#PF Page Fault"
#define MSG_EXECP_COPROC_ERR   "#MF x87 FPU Floating-Point Error (Math Fault)"

#define EXCEPTION_HANDLER(vector, name, code) \
    void name(int b, int c, int d) { \
        int a = *(int *)((void **)&b - 1); \
        early_printk("\nException! --> "MSG_##vector"\n\n"); \
        if (!code) \
            early_printk("EFLAGS:%x CS:%x EIP:%x\n", c, b, a); \
        else \
            early_printk("EFLAGS:%x CS:%x EIP:%x ERR:%x\n", d, c, b, a); \
        while (1) ; \
    }

EXCEPTION_HANDLER(EXECP_DIVIDE,       divide_error,          FALSE);
EXCEPTION_HANDLER(EXECP_DEBUG,        single_step_exception, FALSE);
EXCEPTION_HANDLER(EXECP_NMI,          nmi,                   FALSE);
EXCEPTION_HANDLER(EXECP_BREAKPOINT,   breakpoint_exception,  FALSE);
EXCEPTION_HANDLER(EXECP_OVERFLOW,     overflow,              FALSE);
EXCEPTION_HANDLER(EXECP_BOUNDS,       bounds_check,          FALSE);
EXCEPTION_HANDLER(EXECP_INVAL_OP,     inval_opcode,          FALSE);
EXCEPTION_HANDLER(EXECP_COPROC_NOT,   copr_not_available,    FALSE);
EXCEPTION_HANDLER(EXECP_DOUBLE_FAULT, double_fault,           TRUE);
EXCEPTION_HANDLER(EXECP_COPROC_SEG,   copr_seg_overrun,      FALSE);
EXCEPTION_HANDLER(EXECP_INVAL_TSS,    inval_tss,              TRUE);
EXCEPTION_HANDLER(EXECP_SEG_NOT,      segment_not_present,    TRUE);
EXCEPTION_HANDLER(EXECP_STACK_FAULT,  stack_exception,        TRUE);
EXCEPTION_HANDLER(EXECP_PROTECTION,   general_protection,     TRUE);
EXCEPTION_HANDLER(EXECP_PAGE_FAULT,   page_fault,             TRUE);
EXCEPTION_HANDLER(EXECP_COPROC_ERR,   copr_error,            FALSE);

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

void init_protect()
{
    init_idt_desc(EXECP_DIVIDE,       DA_386IGate, divide_error,          PRIVI_KRNL);
    init_idt_desc(EXECP_DEBUG,        DA_386IGate, single_step_exception, PRIVI_KRNL);
    init_idt_desc(EXECP_NMI,          DA_386IGate, nmi,                   PRIVI_KRNL);
    init_idt_desc(EXECP_BREAKPOINT,   DA_386IGate, breakpoint_exception,  PRIVI_USER);
    init_idt_desc(EXECP_OVERFLOW,     DA_386IGate, overflow,              PRIVI_USER);
    init_idt_desc(EXECP_BOUNDS,       DA_386IGate, bounds_check,          PRIVI_KRNL);
    init_idt_desc(EXECP_INVAL_OP,     DA_386IGate, inval_opcode,          PRIVI_KRNL);
    init_idt_desc(EXECP_COPROC_NOT,   DA_386IGate, copr_not_available,    PRIVI_KRNL);
    init_idt_desc(EXECP_DOUBLE_FAULT, DA_386IGate, double_fault,          PRIVI_KRNL);
    init_idt_desc(EXECP_COPROC_SEG,   DA_386IGate, copr_seg_overrun,      PRIVI_KRNL);
    init_idt_desc(EXECP_INVAL_TSS,    DA_386IGate, inval_tss,             PRIVI_KRNL);
    init_idt_desc(EXECP_SEG_NOT,      DA_386IGate, segment_not_present,   PRIVI_KRNL);
    init_idt_desc(EXECP_STACK_FAULT,  DA_386IGate, stack_exception,       PRIVI_KRNL);
    init_idt_desc(EXECP_PROTECTION,   DA_386IGate, general_protection,    PRIVI_KRNL);
    init_idt_desc(EXECP_PAGE_FAULT,   DA_386IGate, page_fault,            PRIVI_KRNL);
    init_idt_desc(EXECP_COPROC_ERR,   DA_386IGate, copr_error,            PRIVI_KRNL);
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
