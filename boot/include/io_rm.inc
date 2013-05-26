
NR_SEC_PER_TRK  equ  18

r_disp_str:
    push   bp
    mov    bp, [esp + 8]
    push   cx
    mov    cx, [esp + 8]
    push   dx
    mov    dx, [esp + 8]
    push   ax
    push   bx
    push   es

    mov    ax, cs
    mov    es, ax
    mov    ax, 1301h
    mov    bx, 0007h
    int    10h

    pop   es
    pop   bx
    pop   ax
    pop   dx
    pop   cx
    pop   bp
    ret

r_read_sector:
    push   bp
    mov    bp, sp
    push   ax
    push   bx
    push   cx
    push   dx
    push   es

    mov    ax, [bp + 10]
    mov    bl, NR_SEC_PER_TRK
    div    bl
    inc    ah
    mov    cl, ah
    mov    dh, al
    and    dh, 1
    shr    al, 1
    mov    ch, al

    mov    es, [bp + 6]
    mov    bx, [bp + 4]
    mov    dl, 0

    .read:
        mov    al, [bp + 8]
        mov    ah, 02h
        int    13h
        jc     .read

    pop    es
    pop    dx
    pop    cx
    pop    bx
    pop    ax
    pop    bp
    ret

r_reset_floppy:
    push   ax
    push   dx

    xor    ah, ah
    xor    dl, dl
    int    13h

    pop    dx
    pop    ax
    ret

r_kill_motor:
    push   dx
    push   ax

    mov    dx, 03F2h
    mov    al, 0
    out    dx, al

    pop    ax
    pop    dx
    ret
