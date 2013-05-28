PHY_DISP_POS equ 090000h

[section .text]
global  disp_str

disp_str:
    mov    ah, 0Fh
    mov    al, 'S'
    mov    [gs:((80 * 0 + 39) * 2)], ax
    ret