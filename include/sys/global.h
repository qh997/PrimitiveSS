#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "sys/protect.h"

extern int disp_pos;
extern int mem_base;
extern int mem_size;

extern struct desc_seg gdt[];
extern u8 gdtr[];
extern struct desc_gate idt[];
extern u8 idtr[];

#endif
