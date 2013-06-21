#include "tty/console.h"
#include "sys/protect.h"
#include "string.h"
#include "types.h"
#include "sys/interrupts.h"
#include "sys/proto.h"

struct console console_table[NR_CONSOLES];
struct console *current_console;

#define CON_1ST (console_table)
#define CON_END (console_table + NR_CONSOLES)

extern int disp_pos;

void console_init()
{
    memset(console_table, 0, NR_CONSOLES * sizeof(struct console));

    int per_con_size = DISPLAY_SIZE / NR_CONSOLES;

    for (int i = 0; i < NR_CONSOLES; i++) {
        struct console *con = &console_table[i];
        con->cons_start = per_con_size * i;
        con->cons_size = per_con_size;

        //con->crnt_pos =
        con->scrn_start =
        con->cursor = con->cons_start;
    }

    //CON_1ST->crnt_pos = disp_pos / 2;
    CON_1ST->cursor = disp_pos / 2;

    current_console = console_table;
}

struct console *get_console(int index)
{
    if (index < NR_CONSOLES)
        return &console_table[index];
    else
        return NULL;
}

bool is_current_console(struct console *cons)
{
    return (cons == current_console);
}

void scroll_screen(struct console *cons, bool dir)
{

}

static void set_cursor(unsigned int position)
{
    disable_int();
    out_b(CRTC_ADDR_REG, CURSOR_H);
    out_b(CRTC_DATA_REG, (position >> 8) & 0xFF);
    out_b(CRTC_ADDR_REG, CURSOR_L);
    out_b(CRTC_DATA_REG, position & 0xFF);
    enable_int();
}

static void set_screen_start(u32 addr)
{
    disable_int();
    out_b(CRTC_ADDR_REG, START_ADDR_H);
    out_b(CRTC_DATA_REG, (addr >> 8) & 0xFF);
    out_b(CRTC_ADDR_REG, START_ADDR_L);
    out_b(CRTC_DATA_REG, addr & 0xFF);
    enable_int();
}

static void flush()
{
    set_cursor(current_console->cursor);
    set_screen_start(current_console->scrn_start);
}

void output_char(struct console *cons, char ch)
{
    u8 *pos = (u8 *)vir2lin(gdt[INDEX_VIDO], cons->cursor * 2);

    //int cursor_x = (cons->cursor - cons->scrn_start) % SCREEN_WIDTH;
    int cursor_y = (cons->cursor - cons->scrn_start) / SCREEN_WIDTH;

    switch (ch) {
        case '\n':
            cons->cursor = cons->scrn_start + SCREEN_WIDTH * (cursor_y + 1);
            break;
        default:
            *pos++ = ch;
            *pos++ = 0x7;
            cons->cursor++;
            break;
    }

    flush();
}
