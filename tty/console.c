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

#define SCREEN_TO_MEMORY(off) vir2lin(gdt[INDEX_VIDO], off * 2)
#define MEMUSED(cons) (cons->cursor - cons->cons_start)

extern int disp_pos;

void console_init()
{
    memset(console_table, 0, NR_CONSOLES * sizeof(struct console));

    int per_con_size = DISPLAY_SIZE / NR_CONSOLES;
    int buf_com = SCREEN_WIDTH;


    for (int i = 0; i < NR_CONSOLES; i++) {
        struct console *cons = &console_table[i];
        cons->cons_start = per_con_size * i;
        cons->cons_size = per_con_size - buf_com;

        cons->scrn_start = cons->cons_start;
        cons->cursor = cons->cons_start;
        cons->is_full = FALSE;
    }

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
{}

static void set_cursor(u32 position)
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
    u8 *pos = (u8 *)SCREEN_TO_MEMORY(cons->cursor);

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

    if ((!cons->is_full) && (MEMUSED(cons) >= cons->cons_size))
        cons->is_full = TRUE;

    cursor_y = (cons->cursor - cons->scrn_start) / SCREEN_WIDTH;
    if (cursor_y >= SCREEN_LENGTH) {
        if (cons->is_full) {
            memcpy(
                (void *)SCREEN_TO_MEMORY(cons->cons_start),
                (void *)SCREEN_TO_MEMORY(cons->cons_start + SCREEN_WIDTH),
                (MEMUSED(cons) - SCREEN_WIDTH) * 2
            );
            cons->cursor -= SCREEN_WIDTH;
            memset((void *)SCREEN_TO_MEMORY(cons->cursor), 0, SCREEN_WIDTH * 2);
        }
        else
            cons->scrn_start += SCREEN_WIDTH;
    }

    flush();
}
