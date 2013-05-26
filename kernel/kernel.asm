
extern  kernel_init

[section .text]

global _start

_start:
    call   kernel_init

    mov    ah, 0Fh
    mov    al, 'K'
    mov    [gs:((80 * 0 + 39) * 2)], ax

    jmp    $
