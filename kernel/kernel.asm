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

    mov    ah, 0Fh
    mov    al, 'K'
    mov    [gs:((80 * 0 + 39) * 2)], ax

    jmp    $
