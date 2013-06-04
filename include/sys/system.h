#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#define stol(sel) \
    ((gdt[sel >> 3].base_2 << 24) | \
     (gdt[sel >> 3].base_1 << 16) | \
     (gdt[sel >> 3].base_0))
#define vtol(sel, vir) ((u32)(((u32)stol(sel)) + ((u32)vir)))

#endif
