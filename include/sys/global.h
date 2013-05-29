#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "sys/protect.h"

extern int disp_pos;

extern struct descriptor gdt[];
extern u8 gdtr[];
extern u8 idtr[];

#endif
