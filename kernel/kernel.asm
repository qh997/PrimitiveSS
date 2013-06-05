%include "boot/defs.inc"

; Functions
extern  init
extern  kernel_main
extern  ProcA
extern  clock_handler

; Global variables
extern  disp_pos
extern  mem_base
extern  mem_size
extern  gdtr
extern  idtr
extern  tss

[section .text]
global _start
global move_to_user_mode
global clock_handler_a

_start:
    mov    eax, STK_TOP
    mov    esp, eax

    mov    eax, [PHY_DISP_POS]
    mov    [disp_pos], eax
    mov    eax, [PHY_MEM_BASE]
    mov    [mem_base], eax
    mov    eax, [PHY_MEM_SIZE]
    mov    [mem_size], eax

    sgdt   [gdtr]
    call   init
    lgdt   [gdtr]

    lidt   [idtr]

    ;xor    eax, eax
    ;mov    ax, SEL_TSS
    ;ltr    ax

    jmp    SEL_TEXT:new_gdtr

new_gdtr:
    mov    eax, STK_TOP
    mov    esp, eax
    jmp    kernel_main

    jmp    $

move_to_user_mode:
    mov    ax, 0fh
    mov    ds,  ax
    mov    es,  ax
    mov    fs,  ax
    mov    gs,  ax
    mov    eax, esp
    pushf
    mov    dword [esp], 3202h
    popf
    push   0fh
    push   eax
    pushf
    push   07h
    push   ProcA
    iret

clock_handler_a:
    mov    al, 0x20
    out    0x20, al

    call   clock_handler
    iret
    jmp    move_to_user_mode
