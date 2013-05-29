%include "boot/defs.inc"
%include "boot/protect.inc"

org    LOADER_OFFSET
jmp    START

GDT_NONE: DESCRIPTOR        0,               0, 0
GDT_TEXT: DESCRIPTOR        0, KERNEL_SIZE_MAX, DA_CR |DA_32|DA_4K
GDT_DATA: DESCRIPTOR        0, KERNEL_SIZE_MAX, DA_DRW|DA_32|DA_4K
GDT_STAK: DESCRIPTOR    STK_B,           STK_L, DA_DRWA|DA_32
GDT_VIDO: DESCRIPTOR VGA_ADDR,          0ffffh, DA_DRW|DA_DPL3

gdt_len  equ  $ - GDT_NONE
gdt_ptr  dw   gdt_len - 1
         dd   LOADER_ADDR + GDT_NONE

sel_t  equ  GDT_TEXT - GDT_NONE
sel_d  equ  GDT_DATA - GDT_NONE
sel_s  equ  GDT_STAK - GDT_NONE
sel_v  equ  GDT_VIDO - GDT_NONE + SA_RPL3

START:
    mov    ax, cs
    mov    ds, ax
    mov    es, ax
    mov    ss, ax
    mov    sp, LOADER_OFFSET

    R_DISP_STR  0100h, _msg_load_start

    call   r_reset_floppy

    push   01h + LOADER_SECTOR_NR
    push   KERNEL_BIN_SECTOR_NR
    push   KERNEL_BIN_SEG
    push   KERNEL_BIN_OFFSET
    call   r_read_sector
    add    esp, 8

    mov    ebx, 0
    mov    di, _MemChkBuf
    mcloop:
        mov    eax, 0E820h
        mov    ecx, 20
        mov    edx, 0534D4150h
        int    15h
        jc     mcfail
        add    di, 20
        inc    dword [_dwMCRNumber] ; dwMCRNumber：ARDS 的个数
        cmp    ebx, 0
        jne    mcloop
        jmp    mcok
        mcfail:
            mov    dword [_dwMCRNumber], 0
        mcok:

    call   r_kill_motor

    R_DISP_STR  0100h + _msg_load_start_len + 1, _msg_load_ready

    mov    ah, 03h
    xor    bh, bh
    int    0x10
    xor    ax, ax
    mov    al, dh
    xor    bx, bx
    mov    bl, 50h
    mul    bl
    xor    dh, dh
    add    ax, dx
    xor    bx, bx
    mov    bl, 02h
    mul    bx
    mov    [_dwDispPos], ax

    ;; 准备进入保护模式
    lgdt   [gdt_ptr]

    cli

    in     al, 92h
    or     al, 00000010b
    out    92h, al

    mov    eax, cr0
    or     eax, 1
    mov    cr0, eax

    jmp    dword sel_t:(LOADER_ADDR + PM_START)

%include "boot/io_rm.inc"

ALIGN 32
[BITS 32]

PM_START:
    mov    ax, sel_d
    mov    ds, ax
    mov    es, ax
    mov    fs, ax
    mov    ss, ax
    mov    esp, LOADER_ADDR + LOADER_OFFSET
    mov    ax, sel_v
    mov    gs, ax

    _DISP_STR 03h, strPmStart

    _DISP_STR 08h, szMemChkTitle
    call   disp_mem_info
    _DISP_ENTER

    call   setup_paging

    _DISP_STR 08h, szDisKernel
    call   _dispose_kernel

    mov    eax, [dwDispPos]
    mov    dword [PHY_DISP_POS], eax

    jmp    sel_t:(KERNEL_ADDR + KERNEL_OFFSET)

%include "boot/io_pm.inc"
%include "boot/elf.inc"

disp_mem_info:
    push   esi
    push   edi
    push   ecx

    mov    esi, MemChkBuf
    mov    ecx, [dwMCRNumber]            ; for (i=0; i<[MCRNumber]; i++)
    .loop:                               ; {
        mov    edx, 5                    ;     for (j=0; j<5; j++)
        mov    edi, ARDStruct            ;     {
        .1:                              ;
            push   dword [esi]           ;
            _DISP_INT  0fh, dword [esi]  ;         DispInt(MemChkBuf[j*4]); // 显示一个成员
            _DISP_SPACE                  ;         printf(" ");
            pop    eax                   ;
            stosd                        ;         ARDStruct[j*4] = MemChkBuf[j*4];
            add    esi, 4                ;
            dec    edx                   ;
            cmp    edx, 0                ;
            jnz    .1                    ;     }
        _DISP_ENTER                      ;     printf("\n");
        cmp    dword [dwType], 1         ;     if (Type == AddressRangeMemory)
        jne    .2                        ;     {
        mov    eax, [dwBaseAddrLow]      ;
        add    eax, [dwLengthLow]        ;
        cmp    eax, [dwMemSize]          ;         if (BaseAddrLow + LengthLow > MemSize) {
        jb     .2                        ;
        mov    [dwMemSize], eax          ;             MemSize = BaseAddrLow + LengthLow;
        push   eax                       ;
        mov    eax, [dwBaseAddrLow]      ;
        mov    [PHY_MEM_BASE], eax       ;
        mov    eax, [dwLengthLow]        ;
        mov    dword [PHY_MEM_SIZE], eax ;
        pop    eax                       ;         }
        .2:                              ;     }
            loop    .loop                ; }

    _DISP_STR  08h, szRAMSize
    _DISP_INT  0fh, dword [dwMemSize]

    pop    ecx
    pop    edi
    pop    esi
    ret

setup_paging:
    mov    ecx, 1024 * (1 + KERNEL_PTE_NR)
    mov    edi, KERNEL_PDE_BASE
    xor    eax, eax
    .empty:
        stosd
        loop   .empty

    mov    edi, KERNEL_PDE_BASE
    mov    ecx, KERNEL_PTE_NR
    xor    eax, eax
    mov    eax, KERNEL_PTE_BASE | PGN
    .k_pde:
        stosd
        add    eax, 4096
        loop   .k_pde

    mov    edi, KERNEL_PTE_BASE
    mov    ecx, KERNEL_PTE_NR * 1024
    xor    eax, eax
    mov    eax, PGN
    .k_pte:
        stosd
        add    eax, 4096
        loop   .k_pte

    mov    eax, KERNEL_PDE_BASE
    mov    cr3, eax
    mov    eax, cr0
    or     eax, 80000000h
    mov    cr0, eax
    jmp    short .bgp
    .bgp:
        nop
    ret

DATA1:
    _msg_load_start:     db   "Loading ..."
    _msg_load_start_len  equ  $ - _msg_load_start
    _msg_load_ready:     db   "OK"
    _msg_load_ready_len  equ  $ - _msg_load_ready

    _strPmStart:      db  CHAR_ENTER, "Entering Protect Mode", CHAR_ENTER, 0
    _szMemChkTitle:   db  " BaseAddrL  BaseAddrH    LengthL    LengthH       Type", 0Ah, 0
    _szRAMSize:       db  "RAM size: ", 0
    _szDisKernel:     db  "Dispose kernel ...", CHAR_ENTER, 0
    _szReturn:        db  CHAR_ENTER, 0
    _szSpace:         db  " ", 0
    _dwDispPos:       dd  0
    _dwMemSize:       dd  0
    _dwMCRNumber:     dd  0
    _ARDStruct:
        _dwBaseAddrLow:   dd  0
        _dwBaseAddrHigh:  dd  0
        _dwLengthLow:     dd  0
        _dwLengthHigh:    dd  0
        _dwType:          dd  0
    _MemChkBuf: times   256 db  0

    strPmStart       equ  LOADER_ADDR + _strPmStart
    szMemChkTitle    equ  LOADER_ADDR + _szMemChkTitle
    szRAMSize        equ  LOADER_ADDR + _szRAMSize
    szDisKernel      equ  LOADER_ADDR + _szDisKernel
    szReturn         equ  LOADER_ADDR + _szReturn
    szSpace          equ  LOADER_ADDR + _szSpace
    dwDispPos        equ  LOADER_ADDR + _dwDispPos
    dwMemSize        equ  LOADER_ADDR + _dwMemSize
    dwMCRNumber      equ  LOADER_ADDR + _dwMCRNumber
    ARDStruct        equ  LOADER_ADDR + _ARDStruct
        dwBaseAddrLow   equ  LOADER_ADDR + _dwBaseAddrLow
        dwBaseAddrHigh  equ  LOADER_ADDR + _dwBaseAddrHigh
        dwLengthLow     equ  LOADER_ADDR + _dwLengthLow
        dwLengthHigh    equ  LOADER_ADDR + _dwLengthHigh
        dwType          equ  LOADER_ADDR + _dwType
    MemChkBuf  equ  LOADER_ADDR + _MemChkBuf

times  LOADER_SIZE - 5 - ($ - $$)  db  0
LOADER_MAGIC
