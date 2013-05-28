CHAR_ENTER  equ  0ah

extern  disp_pos

[section .text]
global  disp_str

disp_str:
    push   ebp
    mov    ebp, esp

    mov    esi, [ebp + 8]
    mov    edi, [disp_pos]
    mov    ah, 0eh
    cld
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
    mov    [disp_pos], edi
    call   update_cursor

    pop    ebp
    ret

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
