#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include "types.h"

#define DISPLAY_SIZE  (0x10000 / 2)
#define SCREEN_LENGTH 25
#define SCREEN_WIDTH  80

#define NR_CONSOLES 3

#define CRTC_ADDR_REG 0x3D4   /* CRT Controller Registers - Addr Register */
#define CRTC_DATA_REG 0x3D5   /* CRT Controller Registers - Data Register */
#define START_ADDR_H  0xC     /* reg index of video mem start addr (MSB) */
#define START_ADDR_L  0xD     /* reg index of video mem start addr (LSB) */
#define CURSOR_H      0xE     /* reg index of cursor position (MSB) */
#define CURSOR_L      0xF     /* reg index of cursor position (LSB) */
#define V_MEM_BASE    0xB8000 /* base of color video memory */
#define V_MEM_SIZE    0x8000  /* 32K: B8000H -> BFFFFH */

struct console {
    int cons_start;
    int cons_size;

    int scrn_start;
    int cursor;

    bool is_full;
};

void console_init();
struct console *get_console(int index);
bool is_current_console(struct console *con);
void output_char(struct console *cons, char ch);

#endif
