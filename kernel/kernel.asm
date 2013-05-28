%include "boot/defs.inc"

extern  init

extern  disp_pos

[section .text]
global _start

_start:
    mov    eax, 018h
    mov    ss, eax
    mov    eax, STK_TOP
    mov    esp, eax

    mov    eax, [PHY_DISP_POS]
    mov    [disp_pos], eax

    call   init

    jmp    $
