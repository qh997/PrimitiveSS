#ifndef __TTY_H__
#define __TTY_H__

#include "tty/console.h"

struct tty {
    struct console *con;
};

#endif
