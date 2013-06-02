#include "sys/interrupts.h"
#include "sys/protect.h"
#include "stdio.h"

#define MSG_TRAP_DIVIDE       "#DE Divide Error"
#define MSG_TRAP_DEBUG        "#DB RESERVED"
#define MSG_TRAP_NMI          "--  NMI Interrupt"
#define MSG_TRAP_BREAKPOINT   "#BP Breakpoint"
#define MSG_TRAP_OVERFLOW     "#OF Overflow"
#define MSG_TRAP_BOUNDS       "#BR BOUND Range Exceeded"
#define MSG_TRAP_INVAL_OP     "#UD Invalid Opcode (Undefined Opcode)"
#define MSG_TRAP_COPROC_NOT   "#NM Device Not Available (No Math Coprocessor)"
#define MSG_TRAP_DOUBLE_FAULT "#DF Double Fault"
#define MSG_TRAP_COPROC_SEG   "    Coprocessor Segment Overrun (reserved)"
#define MSG_TRAP_INVAL_TSS    "#TS Invalid TSS"
#define MSG_TRAP_SEG_NOT      "#NP Segment Not Present"
#define MSG_TRAP_STACK_FAULT  "#SS Stack-Segment Fault"
#define MSG_TRAP_PROTECTION   "#GP General Protection"
#define MSG_TRAP_PAGE_FAULT   "#PF Page Fault"
#define MSG_TRAP_COPROC_ERR   "#MF x87 FPU Floating-Point Error (Math Fault)"

#define TRAP_HANDLER(excp, name, code) \
    void name(int b, int c, int d) { \
        int a = *(int *)((void **)&b - 1); \
        early_printk("\nException! --> "MSG_##excp"\n\n"); \
        if (!code) \
            early_printk("EFLAGS:%x CS:%x EIP:%x\n", c, b, a); \
        else \
            early_printk("EFLAGS:%x CS:%x EIP:%x ERR:%x\n", d, c, b, a); \
        while (1) ; \
    }

TRAP_HANDLER(TRAP_DIVIDE,       divide_error,          FALSE);
TRAP_HANDLER(TRAP_DEBUG,        single_step_exception, FALSE);
TRAP_HANDLER(TRAP_NMI,          nmi,                   FALSE);
TRAP_HANDLER(TRAP_BREAKPOINT,   breakpoint_exception,  FALSE);
TRAP_HANDLER(TRAP_OVERFLOW,     overflow,              FALSE);
TRAP_HANDLER(TRAP_BOUNDS,       bounds_check,          FALSE);
TRAP_HANDLER(TRAP_INVAL_OP,     inval_opcode,          FALSE);
TRAP_HANDLER(TRAP_COPROC_NOT,   copr_not_available,    FALSE);
TRAP_HANDLER(TRAP_DOUBLE_FAULT, double_fault,           TRUE);
TRAP_HANDLER(TRAP_COPROC_SEG,   copr_seg_overrun,      FALSE);
TRAP_HANDLER(TRAP_INVAL_TSS,    inval_tss,              TRUE);
TRAP_HANDLER(TRAP_SEG_NOT,      segment_not_present,    TRUE);
TRAP_HANDLER(TRAP_STACK_FAULT,  stack_exception,        TRUE);
TRAP_HANDLER(TRAP_PROTECTION,   general_protection,     TRUE);
TRAP_HANDLER(TRAP_PAGE_FAULT,   page_fault,             TRUE);
TRAP_HANDLER(TRAP_COPROC_ERR,   copr_error,            FALSE);

void trap_init()
{
    register_irq_handler(TRAP_DIVIDE,       divide_error         );
    register_irq_handler(TRAP_DEBUG,        single_step_exception);
    register_irq_handler(TRAP_NMI,          nmi                  );
    register_irq_handler(TRAP_BREAKPOINT,   breakpoint_exception );
    register_irq_handler(TRAP_OVERFLOW,     overflow             );
    register_irq_handler(TRAP_BOUNDS,       bounds_check         );
    register_irq_handler(TRAP_INVAL_OP,     inval_opcode         );
    register_irq_handler(TRAP_COPROC_NOT,   copr_not_available   );
    register_irq_handler(TRAP_DOUBLE_FAULT, double_fault         );
    register_irq_handler(TRAP_COPROC_SEG,   copr_seg_overrun     );
    register_irq_handler(TRAP_INVAL_TSS,    inval_tss            );
    register_irq_handler(TRAP_SEG_NOT,      segment_not_present  );
    register_irq_handler(TRAP_STACK_FAULT,  stack_exception      );
    register_irq_handler(TRAP_PROTECTION,   general_protection   );
    register_irq_handler(TRAP_PAGE_FAULT,   page_fault           );
    register_irq_handler(TRAP_COPROC_ERR,   copr_error           );
}
