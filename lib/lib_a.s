extern  disp_pos

[section .text]
global  update_cursor
global  syscall_tmp1

update_cursor:
    push   ebx
    push   edx

    mov    ebx, [disp_pos]
    shr    ebx, 1
    mov    dx, 03D4h
    mov    al, 0eh
    out    dx, al
    nop
    mov    dx, 03D5h
    mov    ax, bx
    shr    ax, 8
    out    dx, al
    nop
    mov    dx, 03D4h
    mov    al, 0fh
    out    dx, al
    nop
    mov    dx, 03D5h
    mov    ax, bx
    out    dx, al
    nop

    pop    edx
    pop    ebx
    ret
