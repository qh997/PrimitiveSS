
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

    push   _msg_load_start
    push   _msg_load_start_len
    push   0300h
    call   __disp_str
    add    esp, 6

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

    call   __kill_motor

    push   _msg_load_ready
    push   _msg_load_ready_len
    push   0402h
    call   __disp_str
    add    esp, 6

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

    push   strPmStart
    call   _disp_str
    add    esp, 4

    jmp    $

%include "io_pm.inc"

DATA1:
    _msg_load_start:     db   "Loading ..."
    _msg_load_start_len  equ  $ - _msg_load_start
    _msg_load_ready:     db   "ready"
    _msg_load_ready_len  equ  $ - _msg_load_ready

    _strPmStart:      db  CHAR_ENTER, CHAR_ENTER, "Entering Protect Mode", 0
    _dwDispPos:       dd  0
    _dwCharColor:     dd  0bh
    _dwMCRNumber:     dd  0
    _ARDStruct:
        _dwBaseAddrLow:   dd  0
        _dwBaseAddrHigh:  dd  0
        _dwLengthLow:     dd  0
        _dwLengthHigh:    dd  0
        _dwType:          dd  0
    _MemChkBuf: times   256 db  0

    strPmStart       equ  LOADER_ADDR + _strPmStart
    dwDispPos        equ  LOADER_ADDR + _dwDispPos
    dwCharColor      equ  LOADER_ADDR + _dwCharColor
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
