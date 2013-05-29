#ifndef __PROTECT_H__
#define __PROTECT_H__

#include "types.h"

#define INDEX_NULL  0 // ┓
#define INDEX_TEXT  1 // ┣ LOADER 里面已经确定了的.
#define INDEX_DATA  2 // ┃
#define INDEX_VIDEO 3 // ┛
#define INDEX_LDT   4

#define SEL_0  (INDEX_NULL  << 3)
#define SEL_T  (INDEX_TEXT  << 3)
#define SEL_D  (INDEX_DATA  << 3)
#define SEL_V ((INDEX_VIDEO << 3) + 3)
#define SEL_1  (INDEX_LDT   << 3)

#define V_MEM_BASE    0xB8000

#define KERNEL_PTE_NR 0x4
#define PROC_LNSPACE 4 // in MB
#define NR_GDT (4 + 2 * (1024 - KERNEL_PTE_NR) / PROC_LNSPACE)
#define NR_IDT 256

struct desc_seg {
    u16 limit_0;
    u16 base_0;
    u8  base_1;
    u8  type_0;
    u8  limit_1_type_1;
    u8  base_2;
} __attribute__((packed));

struct desc_gate {
    u16 offset_0;
    u16 selector;
    u8  pcount;
    u8  attr;
    u16 offset_1;
} __attribute__((packed));;

#endif
