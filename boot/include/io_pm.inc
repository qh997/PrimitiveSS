
_disp_str:
    push   esi
    mov    esi, [esp + 8]
    push   edi

    mov    edi, [dwDispPos]
    mov    ah, [dwCharColor]
    .start:
        lodsb
        test   al, al
        jz     .end
        cmp    al, CHAR_ENTER
        jz     .enter
        mov    [gs:edi], ax
        add    edi, 2
        jmp    .start

        .enter:
            push   eax
            mov    eax, edi
            mov    bl, 80 * 2
            div    bl
            and    eax, 0ffh
            inc    eax
            mov    bl, 160
            mul    bl
            mov    edi, eax
            pop    eax
            jmp    .start

        .end:
            mov    [dwDispPos], edi

    call   _update_cursor

    pop    edi
    pop    esi
    ret

_update_cursor:
    push   ebx
    push   edx

    mov    ebx, [dwDispPos]
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
