#ifndef __PROTO_H__
#define __PROTO_H__

#include "types.h"

void early_printk(char *s);
void update_cursor();

void out_b(u16 port, u8 val);
u8 in_b(u16 port);

#endif
