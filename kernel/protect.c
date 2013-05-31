#include "sys/protect.h"
#include "sys/interrupts.h"
#include "sys/global.h"
#include "types.h"
#include "stdio.h"

#define MSG_EXCP_DIVIDE       "#DE Divide Error"
#define MSG_EXCP_DEBUG        "#DB RESERVED"
#define MSG_EXCP_NMI          "--  NMI Interrupt"
#define MSG_EXCP_BREAKPOINT   "#BP Breakpoint"
#define MSG_EXCP_OVERFLOW     "#OF Overflow"
#define MSG_EXCP_BOUNDS       "#BR BOUND Range Exceeded"
#define MSG_EXCP_INVAL_OP     "#UD Invalid Opcode (Undefined Opcode)"
#define MSG_EXCP_COPROC_NOT   "#NM Device Not Available (No Math Coprocessor)"
#define MSG_EXCP_DOUBLE_FAULT "#DF Double Fault"
#define MSG_EXCP_COPROC_SEG   "    Coprocessor Segment Overrun (reserved)"
#define MSG_EXCP_INVAL_TSS    "#TS Invalid TSS"
#define MSG_EXCP_SEG_NOT      "#NP Segment Not Present"
#define MSG_EXCP_STACK_FAULT  "#SS Stack-Segment Fault"
#define MSG_EXCP_PROTECTION   "#GP General Protection"
#define MSG_EXCP_PAGE_FAULT   "#PF Page Fault"
#define MSG_EXCP_COPROC_ERR   "#MF x87 FPU Floating-Point Error (Math Fault)"

#define EXCEPTION_HANDLER(excp, name, code) \
    void name(int b, int c, int d) { \
        int a = *(int *)((void **)&b - 1); \
        early_printk("\nException! --> "MSG_##excp"\n\n"); \
        if (!code) \
            early_printk("EFLAGS:%x CS:%x EIP:%x\n", c, b, a); \
        else \
            early_printk("EFLAGS:%x CS:%x EIP:%x ERR:%x\n", d, c, b, a); \
        while (1) ; \
    }

EXCEPTION_HANDLER(EXCP_DIVIDE,       divide_error,          FALSE);
EXCEPTION_HANDLER(EXCP_DEBUG,        single_step_exception, FALSE);
EXCEPTION_HANDLER(EXCP_NMI,          nmi,                   FALSE);
EXCEPTION_HANDLER(EXCP_BREAKPOINT,   breakpoint_exception,  FALSE);
EXCEPTION_HANDLER(EXCP_OVERFLOW,     overflow,              FALSE);
EXCEPTION_HANDLER(EXCP_BOUNDS,       bounds_check,          FALSE);
EXCEPTION_HANDLER(EXCP_INVAL_OP,     inval_opcode,          FALSE);
EXCEPTION_HANDLER(EXCP_COPROC_NOT,   copr_not_available,    FALSE);
EXCEPTION_HANDLER(EXCP_DOUBLE_FAULT, double_fault,           TRUE);
EXCEPTION_HANDLER(EXCP_COPROC_SEG,   copr_seg_overrun,      FALSE);
EXCEPTION_HANDLER(EXCP_INVAL_TSS,    inval_tss,              TRUE);
EXCEPTION_HANDLER(EXCP_SEG_NOT,      segment_not_present,    TRUE);
EXCEPTION_HANDLER(EXCP_STACK_FAULT,  stack_exception,        TRUE);
EXCEPTION_HANDLER(EXCP_PROTECTION,   general_protection,     TRUE);
EXCEPTION_HANDLER(EXCP_PAGE_FAULT,   page_fault,             TRUE);
EXCEPTION_HANDLER(EXCP_COPROC_ERR,   copr_error,            FALSE);

static void init_idt_desc(unsigned char excp, u8 desc_type, void *handler, unsigned char privilege)
{
    struct desc_gate *p_gate = &idt[excp];
    u32 base = (u32)handler;

    p_gate->offset_0 = base & 0xFFFF;
    p_gate->selector = SEL_T;
    p_gate->pcount   = 0;
    p_gate->attr     = desc_type | (privilege << 5);
    p_gate->offset_1 = (base >> 16) & 0xFFFF;
}

static void ignore_int()
{
    early_printk("Unknown interrupt\n");
}

void init_protect()
{
    for (int i = 0; i < NR_IDT; i++)
        init_idt_desc(i, DA_386IGate, ignore_int, PRIVI_KRNL);

    init_idt_desc(EXCP_DIVIDE,       DA_386IGate, divide_error,          PRIVI_KRNL);
    init_idt_desc(EXCP_DEBUG,        DA_386IGate, single_step_exception, PRIVI_KRNL);
    init_idt_desc(EXCP_NMI,          DA_386IGate, nmi,                   PRIVI_KRNL);
    init_idt_desc(EXCP_BREAKPOINT,   DA_386IGate, breakpoint_exception,  PRIVI_USER);
    init_idt_desc(EXCP_OVERFLOW,     DA_386IGate, overflow,              PRIVI_USER);
    init_idt_desc(EXCP_BOUNDS,       DA_386IGate, bounds_check,          PRIVI_USER);
    init_idt_desc(EXCP_INVAL_OP,     DA_386IGate, inval_opcode,          PRIVI_KRNL);
    init_idt_desc(EXCP_COPROC_NOT,   DA_386IGate, copr_not_available,    PRIVI_KRNL);
    init_idt_desc(EXCP_DOUBLE_FAULT, DA_386IGate, double_fault,          PRIVI_KRNL);
    init_idt_desc(EXCP_COPROC_SEG,   DA_386IGate, copr_seg_overrun,      PRIVI_KRNL);
    init_idt_desc(EXCP_INVAL_TSS,    DA_386IGate, inval_tss,             PRIVI_KRNL);
    init_idt_desc(EXCP_SEG_NOT,      DA_386IGate, segment_not_present,   PRIVI_KRNL);
    init_idt_desc(EXCP_STACK_FAULT,  DA_386IGate, stack_exception,       PRIVI_KRNL);
    init_idt_desc(EXCP_PROTECTION,   DA_386IGate, general_protection,    PRIVI_KRNL);
    init_idt_desc(EXCP_PAGE_FAULT,   DA_386IGate, page_fault,            PRIVI_KRNL);
    init_idt_desc(EXCP_COPROC_ERR,   DA_386IGate, copr_error,            PRIVI_KRNL);
}

void register_excp_handler(int excp, void *handler)
{
    struct desc_gate *p_gate = &idt[excp];
    u32 base = (u32)handler;

    p_gate->offset_0 = base & 0xFFFF;
    p_gate->offset_1 = (base >> 16) & 0xFFFF;
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
