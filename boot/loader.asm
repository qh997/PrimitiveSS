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

    __DISP_STR  0300h, _msg_load_start

    call   r_reset_floppy

    push   es
    mov    ax, KERNEL_BIN_SEG
    mov    es, ax
    mov    dx, 0000h                  ; drive 0, head 0
    mov    cx, 02h + LOADER_SECTOR_NR ; sector 2 + LOADER_SECTOR, track 0
    mov    bx, KERNEL_BIN_OFFSET      ; address
    mov    ah, 02h                    ; service 2
    mov    al, KERNEL_BIN_SECTOR_NR   ; nr of sectors
    int    13h
    pop    es

    __DISP_STR  0400h, _msg_get_mem

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

    __DISP_STR  0502h, _msg_load_ready

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

    push   03h
    push   strPmStart
    call   _disp_str
    add    esp, 8

    call   _disp_enter

    push   08h
    push   0ffffffffh
    call   _disp_int
    add    esp, 8

    call   _disp_enter

    jmp    $

%include "io_pm.inc"

DATA1:
    _msg_load_start:     db   "Loading kernrl ..."
    _msg_load_start_len  equ  $ - _msg_load_start
    _msg_get_mem:        db   "Get memory info ..."
    _msg_get_mem_len     equ  $ - _msg_get_mem
    _msg_load_ready:     db   "ready"
    _msg_load_ready_len  equ  $ - _msg_load_ready

    _strPmStart:      db  CHAR_ENTER, CHAR_ENTER, "Entering Protect Mode", CHAR_ENTER, 0
    _szReturn:        db  CHAR_ENTER, 0
    _szSpace:         db  " ", 0
    _dwDispPos:       dd  0
    _dwMCRNumber:     dd  0
    _ARDStruct:
        _dwBaseAddrLow:   dd  0
        _dwBaseAddrHigh:  dd  0
        _dwLengthLow:     dd  0
        _dwLengthHigh:    dd  0
        _dwType:          dd  0
    _MemChkBuf: times   256 db  0

    strPmStart       equ  LOADER_ADDR + _strPmStart
    szReturn         equ  LOADER_ADDR + _szReturn
    szSpace          equ  LOADER_ADDR + _szSpace
    dwDispPos        equ  LOADER_ADDR + _dwDispPos
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
