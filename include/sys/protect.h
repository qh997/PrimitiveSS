#ifndef __PROTECT_H__
#define __PROTECT_H__

#include "types.h"

struct descriptor {
    u16 limit_0;
    u16 base_0;
    u8  base_1;
    u8  type_0;
    u8  limit_1_type_1;
    u8  base_2;
} __attribute__((packed));

#define NR_GDT 256

#endif
