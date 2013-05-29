#include "sys/protect.h"

int disp_pos;

struct descriptor gdt[NR_GDT];
u8 gdtr[6];
u8 idtr[6];
