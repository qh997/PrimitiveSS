
%include "load.inc"

%macro disp_str 2
    push   ax
    push   bx
    push   bp
    push   cx

    mov    ax, 1301h
    mov    bx, 0007h
    mov    bp, %1
    mov    cx, %2
    int    10h

    pop   cx
    pop   bp
    pop   bx
    pop   ax
%endmacro

org    07c00h
jmp    short START

STACK_TOP  equ  07c00h

START:
    mov    ax, cs
    mov    ds, ax
    mov    es, ax
    mov    ss, ax
    mov    esp, STACK_TOP

    mov    ax, 0600h  ; ┓
    mov    bx, 0700h  ; ┃
    mov    cx, 0      ; ┣ 清屏
    mov    dx, 0184fh ; ┃
    int    10h        ; ┛

    mov    ax, 0200h
    mov    bx, 0
    mov    dx, 0
    int    10h

    disp_str msg_boot, msg_boot_len

    xor    ah, ah ; ┓
    xor    dl, dl ; ┣ 软驱复位
    int    13h    ; ┛

    mov    ax, LOADER_SEG     ; ┓
    mov    es, ax             ; ┃
    mov    bx, LOADER_OFF     ; ┃
    mov    ax, 2              ; ┃
    mov    cl, LOADER_SZE     ; ┃
    call   ReadSector         ; ┻ 将第 ax 个扇区开始的 cl 个扇区读入 es:bx 中

    jmp    LOADER_SEG:LOADER_OFF

ReadSector:
    push   bp
    mov    bp, sp
    sub    esp, 2

    mov    byte [bp - 2], cl
    push   bx
    mov    bl, 1
    div    bl
    inc    ah
    mov    cl, ah
    mov    dh, al
    shr    al, 1
    mov    ch, al
    and    dh, 1

    pop    bx
    mov    dl, 0
    .GoOnReading:
        mov    ah, 2
        mov    al, byte [bp - 2]
        int    13h
        jc     .GoOnReading
    add    esp, 2
    pop    bp

    ret

msg_boot:       db   "Booting ..."
msg_boot_len    equ  11

times  510 - ($ - $$)  db  0
dw  0xaa55
