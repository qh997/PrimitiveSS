#include "sys/proc.h"
#include "sys/sched.h"
#include "tty/console.h"
#include "tty/tty.h"
#include "types.h"
#include "string.h"
#include "tty/keyboard.h"

u8 tty_stack[DEFAULT_STACK_SIZE];
struct tty tty_table[NR_CONSOLES];

#define TTY_1ST (tty_table)
#define TTY_END (tty_table + NR_CONSOLES)

void tty_init()
{
    console_init();
    memset(tty_table, 0, NR_CONSOLES * sizeof(struct tty));

    for (int i = 0; i < NR_CONSOLES; i++) {
        tty_table[i].console = get_console(i);
    }
}

static void tty_write(struct tty *tty, struct proc_msg *msg)
{
    char buf;
    char *pbuf = (char *)proc2linear(msg->sender, msg->content);

    output_char(tty->console, msg->sender + '0');
    while ((*(char *)proc2linear(TASK_TTY, &buf) = *(char *)pbuf++)) {
        output_char(tty->console, buf);
    }
}

void task_tty()
{
    tty_init();

    struct proc_msg msg;

    while (TRUE) {
        send_recv(RECV, ANY, &msg);

        switch (msg.type) {
            case WRITE:
                tty_write(&tty_table[msg.num], &msg);
                msg.type = RECEIPT;
                send_recv(SEND, msg.sender, &msg);
                break;

            case INT:
                break;

            default:
                break;
        }
    }
}
