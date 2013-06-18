#include "tty/keyboard.h"
#include "tty/keymap.h"
#include "sys/interrupts.h"
#include "sys/proto.h"

static struct s_kb kb_in;

static int code_with_E0;
static int shift_l;     /* l shift state */
static int shift_r;     /* r shift state */
static int alt_l;       /* l alt state   */
static int alt_r;       /* r left state  */
static int ctrl_l;      /* l ctrl state  */
static int ctrl_r;      /* l ctrl state  */
static int caps_lock;   /* Caps Lock     */
static int num_lock;    /* Num Lock      */
static int scroll_lock; /* Scroll Lock   */
static int column;

extern void hwint01(int irq);

static void kb_wait()
{
    u8 kb_stat;

    do {
        kb_stat = in_b(KB_CMD);
    } while (kb_stat & 0x02);
}

static void kb_ack()
{
    u8 kb_read;

    do {
        kb_read = in_b(KB_DATA);
    } while ((kb_read = !KB_ACK));
}

static void set_leds()
{
    u8 leds = (caps_lock << 2) | (num_lock << 1) | scroll_lock;

    kb_wait();
    out_b(KB_DATA, LED_CODE);
    kb_ack();

    kb_wait();
    out_b(KB_DATA, leds);
    kb_ack();
}

void keyboard_handler()
{
    u8 scan_code = in_b(KB_DATA);

    if (kb_in.count < KB_IN_BYTES) {
        *(kb_in.p_head) = scan_code;
        kb_in.p_head++;
        kb_in.count++;
        if (kb_in.p_head == kb_in.buf + KB_IN_BYTES)
            kb_in.p_head = kb_in.buf;
    }
}

void init_keyboard()
{
    kb_in.count = 0;
    kb_in.p_head = kb_in.p_tail = kb_in.buf;

    code_with_E0 = 0;
    shift_l = shift_r = 0;
    alt_l = alt_r = 0;
    ctrl_l = ctrl_r = 0;

    caps_lock = 0;
    num_lock = 1;
    scroll_lock = 0;

    column = 0;

    set_leds();

    register_irq_handler(INT_KEYBOARD, hwint01);
    register_hwirq_handler(HWIRQ_KEYBOARD, keyboard_handler);
    enable_hwirq(INT_KEYBOARD);
}
