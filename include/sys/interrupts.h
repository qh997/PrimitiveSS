#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#define INT_M_CTL     0x20
#define INT_M_CTLMASK 0x21
#define INT_S_CTL     0xA0
#define INT_S_CTLMASK 0xA1

/* 中断向量 */
#define TRAP_DIVIDE       0x0
#define TRAP_DEBUG        0x1
#define TRAP_NMI          0x2
#define TRAP_BREAKPOINT   0x3
#define TRAP_OVERFLOW     0x4
#define TRAP_BOUNDS       0x5
#define TRAP_INVAL_OP     0x6
#define TRAP_COPROC_NOT   0x7
#define TRAP_DOUBLE_FAULT 0x8
#define TRAP_COPROC_SEG   0x9
#define TRAP_INVAL_TSS    0xA
#define TRAP_SEG_NOT      0xB
#define TRAP_STACK_FAULT  0xC
#define TRAP_PROTECTION   0xD
#define TRAP_PAGE_FAULT   0xE
#define TRAP_COPROC_ERR   0x10

#define INT_PIC_M         0x20
#define INT_CLOCK         INT_PIC_M + 0
#define INT_KEYBOARD      INT_PIC_M + 1
#define INT_CASCADE       INT_PIC_M + 2  /* cascade enable for 2nd AT controller */
#define INT_ETHER         INT_PIC_M + 3  /* default ethernet interrupt vector */
#define INT_SECONDARY     INT_PIC_M + 3  /* RS232 interrupt vector for port 2 */
#define INT_RS232         INT_PIC_M + 4  /* RS232 interrupt vector for port 1 */
#define INT_XT_WINI       INT_PIC_M + 5  /* xt winchester */
#define INT_FLOPPY        INT_PIC_M + 6  /* floppy disk */
#define INT_PRINTER       INT_PIC_M + 7
#define INT_PIC_S         INT_PIC_M + 8
#define INT_AT_WINI       INT_PIC_S + 6 /* at winchester */

/* 8253/8254 PIT */
#define TIMER0         0x40     /* I/O port for timer channel 0 */
#define TIMER_MODE     0x43     /* I/O port for timer mode control */
#define RATE_GENERATOR 0x34     /*    00    -    11    -    010   -  0  */
                                /* Counter0 - L then H - rate gen - bin */
#define TIMER_FREQ     1193181L /* clock frequency for timer in PC and AT */
#define HZ             100      /* clock freq (software settable on IBM-PC) */

typedef void (*irq_handler)(int irq);
//typedef void (*excp_handler)(int b, int c, int d);

extern irq_handler irq_table[];

extern void PIC8259A_init();
void trap_init();

extern int disable_irq(int irq);
extern void enable_irq(int irq);
extern void register_irq_handler(int vector, void *handler);

#endif
