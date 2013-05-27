%include "defs.inc"
%include "protect.inc"

org    LOADER_OFFSET
jmp    START

GDT_NONE: DESCRIPTOR        0,       0, 0
GDT_TEXT: DESCRIPTOR        0, 0fffffh, DA_CR |DA_32|DA_4K
GDT_DATA: DESCRIPTOR        0, 0fffffh, DA_DRW|DA_32|DA_4K
GDT_VIDO: DESCRIPTOR VGA_ADDR,  0ffffh, DA_DRW|DA_DPL3

gdt_len  equ  $ - GDT_NONE
gdt_ptr  dw   gdt_len - 1
         dd   LOADER_ADDR + GDT_NONE

sel_t  equ  GDT_TEXT - GDT_NONE
sel_d  equ  GDT_DATA - GDT_NONE
sel_v  equ  GDT_VIDO - GDT_NONE + SA_RPL3

START:
    mov    ax, cs
    mov    ds, ax
    mov    es, ax
    mov    ss, ax
    mov    sp, LOADER_OFFSET

    R_DISP_STR  0300h, _msg_load_start

    call   r_reset_floppy

    push   01h + LOADER_SECTOR_NR
    push   KERNEL_BIN_SECTOR_NR
    push   KERNEL_BIN_SEG
    push   KERNEL_BIN_OFFSET
    call   r_read_sector
    add    esp, 8

    R_DISP_STR  0400h, _msg_get_mem

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

    R_DISP_STR  0502h, _msg_load_ready

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

%include "io_rm.inc"

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
    _DISP_ENTER

    _DISP_STR 08h, szMemChkTitle
    call   DispMemInfo

    call   _dispose_kernel

    jmp    sel_t:(KERNEL_ADDR + KERNEL_OFFSET)

%include "io_pm.inc"
%include "elf.inc"

DispMemInfo:
    push   esi
    push   edi
    push   ecx

    mov    esi, MemChkBuf
    mov    ecx, [dwMCRNumber]           ; for (i=0; i<[MCRNumber]; i++)
    .loop:                              ; {
        mov    edx, 5                   ;     for (j=0; j<5; j++)
        mov    edi, ARDStruct           ;     {
        .1:                             ;
            push   dword [esi]          ;
            _DISP_INT  0fh, dword [esi] ;         DispInt(MemChkBuf[j*4]); // 显示一个成员
            _DISP_SPACE                 ;         printf(" ");
            pop    eax                  ;
            stosd                       ;         ARDStruct[j*4] = MemChkBuf[j*4];
            add    esi, 4               ;
            dec    edx                  ;
            cmp    edx, 0               ;
            jnz    .1                   ;     }
        _DISP_ENTER                     ;     printf("\n");
        cmp    dword [dwType], 1        ;     if (Type == AddressRangeMemory)
        jne    .2                       ;     {
        mov    eax, [dwBaseAddrLow]     ;
        add    eax, [dwLengthLow]       ;
        cmp    eax, [dwMemSize]         ;         if (BaseAddrLow + LengthLow > MemSize)
        jb     .2                       ;
        mov    [dwMemSize], eax         ;             MemSize = BaseAddrLow + LengthLow;
        .2:                             ;     }
            loop    .loop               ; }

    _DISP_ENTER
    _DISP_STR  08h, szRAMSize
    _DISP_INT  0fh, dword [dwMemSize]

    pop    ecx
    pop    edi
    pop    esi
    ret

DATA1:
    _msg_load_start:     db   "Loading kernrl ..."
    _msg_load_start_len  equ  $ - _msg_load_start
    _msg_get_mem:        db   "Get memory info ..."
    _msg_get_mem_len     equ  $ - _msg_get_mem
    _msg_load_ready:     db   "ready"
    _msg_load_ready_len  equ  $ - _msg_load_ready

    _strPmStart:      db  CHAR_ENTER, CHAR_ENTER, "Entering Protect Mode", CHAR_ENTER, 0
    _szMemChkTitle:   db  " BaseAddrL  BaseAddrH    LengthL    LengthH       Type", 0Ah, 0
    _szRAMSize:       db  "RAM size: ", 0
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
