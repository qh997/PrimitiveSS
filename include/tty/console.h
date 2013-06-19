#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#define SCREEN_LENGTH 25
#define SCREEN_WIDTH  80

#define NR_CONSOLES 3

struct console {
    int con_start;
    int con_size;

    int crnt_pos;
    int scrn_start;
    int cursor;
};

void console_init();
struct console *get_console(int index);

#endif
