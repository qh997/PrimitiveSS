
_disp_str:
    mov    eax, [esp + 8]
    push   esi
    mov    esi, [esp + 8]
    push   edi

    mov    edi, [dwDispPos]
    shl    eax, 8
    and    ax, 0ff00h
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

_disp_int:
    push   eax
    mov    eax, [esp + 12]
    push   edx
    mov    edx, [esp + 12]
    push   edi
    push   ebx

    shl    eax, 8
    and    ax, 0ff00h
    mov    edi, [dwDispPos]
    mov    al, '0'
    mov    [gs:edi], ax
    add    edi, 2
    mov    al, 'x'
    mov    [gs:edi], ax
    add    edi, 2
    mov    [dwDispPos], edi
    mov    bh, ah

    mov    eax, edx
    shr    eax, 24
    mov    ah, bh
    call   _disp_al

    mov    eax, edx
    shr    eax, 16
    mov    ah, bh
    call   _disp_al

    mov    eax, edx
    shr    eax, 8
    mov    ah, bh
    call   _disp_al

    mov    eax, edx
    mov    ah, bh
    call   _disp_al

    pop    ebx
    pop    edi
    pop    edx
    pop    eax
    ret

_disp_enter:
    push   szReturn
    call   _disp_str
    add    esp, 4
    ret

_disp_space:
    push   szSpace
    call   _disp_str
    add    esp, 4
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

_disp_al:
    push   edi
    push   edx
    push   ecx

    mov    edi, [dwDispPos]
    mov    dl, al
    shr    al, 4
    mov    ecx, 2
    .start:
        and    al, 0fh
        cmp    al, 9
        ja     .a
        add    al, '0'
        jmp    .end
        .a:
        sub    al, 0ah
        add    al, 'A'
        .end:
        mov    [gs:edi], ax
        add    edi, 2
        mov    al, dl
        loop   .start
    mov    [dwDispPos], edi
    call   _update_cursor

    pop    ecx
    pop    edx
    pop    edi
    ret

_memcpy:
    push   ebp
    mov    ebp, esp

    push   esi
    push   edi
    push   ecx

    mov    edi, [ebp + 8]
    mov    esi, [ebp + 12]
    mov    ecx, [ebp + 16]
    .1:
        cmp    ecx, 0
        jz     .2

        mov    al, [ds:esi]
        inc    esi
        mov    byte [es:edi], al
        inc    edi

        dec    ecx
        jmp    .1
    .2:
        mov    eax, [ebp + 8]  ; 返回值

    pop    ecx
    pop    edi
    pop    esi
    mov    esp, ebp
    pop    ebp

    ret
