%include "defs.inc"

extern  kernel_init

[section .text]

global _start

_start:
    mov    eax, 018h
    mov    ss, eax
    mov    eax, STK_TOP
    mov    esp, eax

    call   kernel_init

    jmp    $
