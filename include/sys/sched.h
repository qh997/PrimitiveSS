#ifndef __SCHED_H__
#define __SCHED_H__

#include "sys/protect.h"

#define _LDT(n) ((((u32)n) << 4) + SEL_1ST_LDT)
#define _TSS(n) ((((u32)n) << 4) + SEL_1ST_TSS)
    
#define ltr(n) __asm__("ltr %%ax"::"a" (_TSS(n)))
#define lldt(n) __asm__("lldt %%ax"::"a" (_LDT(n)))

#endif
