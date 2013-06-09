INT_VECTOR_SYS_CALL  equ  0x80

extern  disp_pos
extern  disp_int

bits  32
[section .text]
global  update_cursor
global  pmsg_send
global  pmsg_receive

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

pmsg_send:
    mov    eax, 1
    mov    ebx, [esp + 4]
    mov    ecx, [esp + 8]
    mov    edx, 0
    int    INT_VECTOR_SYS_CALL
    ret

pmsg_receive:
    mov    eax, 2
    mov    ebx, [esp + 4]
    mov    ecx, [esp + 8]
    mov    edx, 0
    int    INT_VECTOR_SYS_CALL
    ret
