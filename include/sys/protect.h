#ifndef __PROTECT_H__
#define __PROTECT_H__

#include "types.h"

/* 选择子类型值说明 */
#define SA_RPL_MASK 0xFFFC
#define SA_RPL0     0
#define SA_RPL1     1
#define SA_RPL2     2
#define SA_RPL3     3

#define SA_TI_MASK  0xFFFB
#define SA_TIG      0
#define SA_TIL      4

/* 描述符类型值 */
#define DA_32           0x4000  // 32 位段
#define DA_LIMIT_4K     0x8000  // 段界限粒度为 4K 字节
#define LIMIT_4K_SHIFT  12
#define DA_DPL0         0x00    // DPL = 0
#define DA_DPL1         0x20    // DPL = 1
#define DA_DPL2         0x40    // DPL = 2
#define DA_DPL3         0x60    // DPL = 3
/* 存储段描述符类型值 */
#define DA_DR           0x90    // 存在的只读数据段类型值
#define DA_DRW          0x92    // 存在的可读写数据段属性值
#define DA_DRWA         0x93    // 存在的已访问可读写数据段类型值
#define DA_C            0x98    // 存在的只执行代码段属性值
#define DA_CR           0x9A    // 存在的可执行可读代码段属性值
#define DA_CCO          0x9C    // 存在的只执行一致代码段属性值
#define DA_CCOR         0x9E    // 存在的可执行可读一致代码段属性值
/* 系统段描述符类型值 */
#define DA_LDT          0x82    // 局部描述符表段类型值
#define DA_TaskGate     0x85    // 任务门类型值
#define DA_386TSS       0x89    // 可用 386 任务状态段类型值
#define DA_386CGate     0x8C    // 386 调用门类型值
#define DA_386IGate     0x8E    // 386 中断门类型值
#define DA_386TGate     0x8F    // 386 陷阱门类型值

/* 权限 */
#define PRIVI_KRNL 0
#define PRIVI_TASK 1
#define PRIVI_USER 3

#define INDEX_NULL    0 // ┓
#define INDEX_TEXT    1 // ┣ LOADER 里已确定
#define INDEX_DATA    2 // ┃
#define INDEX_VIDO    3 // ┛
#define INDEX_1ST_LDT 4
#define INDEX_1ST_TSS 5

#define SEL_NULL     (INDEX_NULL    << 3)
#define SEL_TEXT     (INDEX_TEXT    << 3)
#define SEL_DATA     (INDEX_DATA    << 3)
#define SEL_VIDO    ((INDEX_VIDO    << 3) + SA_RPL3)
#define SEL_1ST_LDT  (INDEX_1ST_LDT << 3)
#define SEL_1ST_TSS  (INDEX_1ST_TSS << 3)

#define KERNEL_PTE_NR 0x4
#define NR_GDT (4 + 2 * (1024 - KERNEL_PTE_NR))
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
} __attribute__((packed));

struct tss {
    u32 backlink;
    u32 esp0;
    u32 ss0;
    u32 esp1;
    u32 ss1;
    u32 esp2;
    u32 ss2;
    u32 cr3;
    u32 eip;
    u32 eflags;
    u32 eax;
    u32 ecx;
    u32 edx;
    u32 ebx;
    u32 esp;
    u32 ebp;
    u32 esi;
    u32 edi;
    u32 es;
    u32 cs;
    u32 ss;
    u32 ds;
    u32 fs;
    u32 gs;
    u32 ldt;
    u16 trap;
    u16 iobase;
} __attribute__((packed));

void init_desc(struct desc_seg *p_desc, u32 base, u32 limit, u16 attribute);
void protect_init();

#endif
