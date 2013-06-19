#include "sys/proc.h"
#include "sys/sched.h"
#include "tty/console.h"
#include "tty/tty.h"
#include "types.h"
#include "string.h"

u8 tty_stack[DEFAULT_STACK_SIZE];
struct tty tty_table[NR_CONSOLES];

#define TTY_1ST (tty_table)
#define TTY_END (tty_table + NR_CONSOLES)

void tty_init()
{
    console_init();
    memset(tty_table, 0, NR_CONSOLES * sizeof(struct tty));

    for (int i = 0; i < NR_CONSOLES; i++) {
        tty_table[i].con = get_console(i);
    }
}

static void tty_do_read(struct tty *t)
{
}

static void tty_do_write(struct tty *t)
{
}

void task_tty()
{
    early_printk("tty task\n");
    tty_init();

    struct proc_msg msg;

    while (TRUE) {
        for (struct tty *t = TTY_1ST; t < TTY_END; t++) {
            tty_do_read(t);
            tty_do_write(t);
        }

        send_recv(RECV, ANY, &msg);

        switch (msg.type) {
            case WRITE:
                break;

            case INT:
                break;

            default:
                break;
        }
    }
}
