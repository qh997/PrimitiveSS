#include "tty/console.h"
#include "sys/protect.h"
#include "string.h"

struct console console_table[NR_CONSOLES];
struct console *current_console;

#define CON_1ST (console_table)
#define CON_END (console_table + NR_CONSOLES)

extern int disp_pos;

void console_init()
{
    memset(console_table, 0, NR_CONSOLES * sizeof(struct console));

    int per_con_size = 0xffff / NR_CONSOLES;
    early_printk("total_row = %d\n", per_con_size);

    for (int i = 0; i < NR_CONSOLES; i++) {
        struct console *con = &console_table[i];
        con->con_start = per_con_size * i;
        con->con_size = per_con_size - 1;

        con->crnt_pos =
        con->scrn_start =
        con->cursor = con->con_start;
    }

    CON_1ST->crnt_pos = disp_pos;
    CON_1ST->cursor = disp_pos;

    current_console = console_table;
}

struct console *get_console(int index)
{
    if (index < NR_CONSOLES)
        return &console_table[index];
    else
        return NULL;
}
