CHAR_ENTER  equ  0ah

INT_M_CTL      equ  0x20
INT_M_CTLMASK  equ  0x21
INT_S_CTL      equ  0xA0
INT_S_CTLMASK  equ  0xA1

extern  disp_pos

[section .text]
global  update_cursor
global  disable_irq
global  enable_irq

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

disable_irq:
    mov    ecx, [esp + 4]        ; ecx = irq
    sub    ecx, 20h
    pushf                        ;
    cli                          ; 关中断，将在 iretd 时打开
    mov    ah, 1                 ;
    rol    ah, cl                ; ah = 1 << (irq % 8)
    cmp    cl, 8                 ;
    jae    disable_8             ; if (cl < 8) // 主 8259A
    disable_0:                   ; {
        in     al, INT_M_CTLMASK ;
        test   al, ah            ;     if (中断已经禁用)
        jnz    dis_already       ;         return 0
        or     al, ah            ;
        out    INT_M_CTLMASK, al ;     屏蔽该中断
        popf                     ;
        mov    eax, 1            ;     return 1
        ret                      ; }
    disable_8:                   ; else { // 从 8259A
        in     al, INT_S_CTLMASK ;
        test   al, ah            ;     if (中断已经禁用)
        jnz    dis_already       ;         return 0
        or     al, ah            ;
        out    INT_S_CTLMASK, al ;     屏蔽该中断
        popf                     ;
        mov    eax, 1            ;     return 1
        ret                      ; }
    dis_already:
        popf
        xor    eax, eax
        ret

enable_irq:
    mov    ecx, [esp + 4]
    sub    ecx, 20h
    pushf
    cli
    mov    ah, ~1
    rol    ah, cl
    cmp    cl, 8
    jae    enable_8
    enable_0:
        in     al, INT_M_CTLMASK
        and    al, ah
        out    INT_M_CTLMASK, al
        popf
        ret
    enable_8:
        in     al, INT_S_CTLMASK
        and    al, ah
        out    INT_S_CTLMASK, al
        popf
        ret
