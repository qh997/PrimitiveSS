
_disp_str:
    push   esi
    mov    esi, [esp + 12]
    push   edi
    mov    edi, [esp + 12]

    mov    ah, 0bh
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
            mov    eax, edi

    pop    edi
    pop    esi
    ret
