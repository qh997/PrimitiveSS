
extern  kernel_init

[section .text]

global _start

_start:
    call   kernel_init
    jmp    $
