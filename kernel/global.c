#include "sys/protect.h"

int disp_pos;
int mem_base;
int mem_size;

struct desc_seg gdt[NR_GDT];
u8 gdtr[6];
struct desc_gate idt[NR_IDT];
u8 idtr[6];
