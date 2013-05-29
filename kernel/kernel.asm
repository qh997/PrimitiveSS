%include "boot/defs.inc"

; Functions
extern  init

; Global variables
extern  disp_pos
extern  gdtr
extern  idtr

[section .text]
global _start

_start:
    mov    eax, 018h
    mov    ss, eax
    mov    eax, STK_TOP
    mov    esp, eax

    mov    eax, [PHY_DISP_POS]
    mov    [disp_pos], eax

    sgdt   [gdtr]
    call   init
    lgdt   [gdtr]

    jmp    8:new_gdtr

new_gdtr:
    jmp    $
