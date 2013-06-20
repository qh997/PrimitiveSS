#ifndef __TTY_H__
#define __TTY_H__

#include "tty/console.h"

#define TTY_IN_BYTES 256
#define TTY_OUT_BUF_LEN 2

struct tty {
    u32 ibuf[TTY_IN_BYTES]; // TTY 输入缓冲区
    u32 *ibuf_head;         // 指向缓冲区中下一个空闲位置
    u32 *ibuf_tail;         // 指向键盘任务应处理的键值
    int ibuf_cnt;           // 缓冲区中已经填充了多少

    struct console *console;
};

void in_process(struct tty *tty, u32 key);

#endif
