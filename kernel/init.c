#include "sys/global.h"
#include "string.h"

void disp_str(char *s);

void init()
{
    disp_str("kernel init.\n");

    memcpy((void *)&gdt, (void *)(*(u32 *)&gdtr[2]), *(u16 *)(&gdtr[0]) + 1);

    *(u16 *)(&gdtr[0]) = NR_GDT * sizeof(struct descriptor) - 1;
    *(u32 *)(&gdtr[2]) = (u32)&gdt;
}
