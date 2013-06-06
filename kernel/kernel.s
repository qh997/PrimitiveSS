%include "boot/defs.inc"

; Functions
extern  init
extern  kernel_main

; Global variables
extern  disp_pos
extern  mem_base
extern  mem_size
extern  gdtr
extern  idtr

[section .text]
global _start

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

    jmp    SEL_TEXT:new_gdtr

new_gdtr:
    mov    eax, STK_TOP
    mov    esp, eax
    jmp    kernel_main

    jmp    $
