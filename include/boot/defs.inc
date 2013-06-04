
VGA_SEG               equ  0B800h
VGA_ADDR              equ  VGA_SEG * 10h

BOOT_PRE_SEG          equ  07c0h
BOOT_PRE_ADDR         equ  BOOT_PRE_SEG * 10h
BOOT_SEG              equ  09000h
BOOT_OFFSET           equ  0200h
BOOT_ADDR             equ  BOOT_SEG * 10h

LOADER_SEG            equ  BOOT_SEG + BOOT_OFFSET / 10h + 20h
LOADER_OFFSET         equ  0400h
LOADER_ADDR           equ  LOADER_SEG * 10h
LOADER_SECTOR_NR      equ  5
LOADER_SIZE           equ  LOADER_SECTOR_NR * 200h

KERNEL_BIN_SEG        equ  04000h
KERNEL_BIN_OFFSET     equ  00h
KERNEL_BIN_ADDR       equ  KERNEL_BIN_SEG * 10h
KERNEL_BIN_SECTOR_NR  equ  20
KERNEL_BIN_SIZE       equ  KERNEL_BIN_SECTOR_NR * 200h

KERNEL_SEG            equ  00h
KERNEL_OFFSET         equ  06000h
KERNEL_ADDR           equ  KERNEL_SEG * 10h

KERNEL_PDE_BASE       equ  00h
KERNEL_PTE_BASE       equ  KERNEL_PDE_BASE + 01000h
KERNEL_PTE_NR         equ  04h
KERNEL_SIZE_MAX       equ  01000h * KERNEL_PTE_NR ; unit: 4KB
STK_TOP               equ  KERNEL_SEG + KERNEL_OFFSET

PHY_DISP_POS          equ  090000h
PHY_MEM_BASE          equ  PHY_DISP_POS + 04h
PHY_MEM_SIZE          equ  PHY_MEM_BASE + 04h

SEL_NULL  equ  00h
SEL_TEXT  equ  08h
SEL_DATA  equ  10h
SEL_VIDO  equ  18h + 3

%define  LOADER_MAGIC  db  "81192"
LOADER_MAGIC_NR  equ  5

CHAR_ENTER  equ  0ah

%macro R_DISP_STR 2
    push   %2
    push   %2_len
    push   %1
    call   r_disp_str
    add    esp, 6
%endmacro

%macro _DISP_STR 2
    push   %1
    push   %2
    call   _disp_str
    add    esp, 8
%endmacro

%macro _DISP_SPACE 0
    push   0h
    push   szSpace
    call   _disp_str
    add    esp, 8
%endmacro

%macro _DISP_ENTER 0
    push   0h
    push   szReturn
    call   _disp_str
    add    esp, 8
%endmacro

%macro _DISP_INT 2
    push   %1
    push   %2
    call   _disp_int
    add    esp, 8
%endmacro
