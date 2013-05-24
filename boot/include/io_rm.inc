
__disp_str:
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

__kill_motor:
    push   dx
    push   ax

    mov    dx, 03F2h
    mov    al, 0
    out    dx, al

    pop    ax
    pop    dx
    ret
