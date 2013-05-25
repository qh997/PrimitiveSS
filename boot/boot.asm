%include "defs.inc"

org    BOOT_ADDR + BOOT_OFFSET
jmp    short START

START:
    mov    ax, BOOT_PRE_SEG
    mov    ds, ax
    mov    ax, BOOT_SEG
    mov    es, ax
    xor    si, si
    mov    di, BOOT_OFFSET
    mov    cx, 256
    rep    movsw
    jmp    BOOT_SEG:GO

GO:
    mov    ax, cs
    mov    ds, ax
    mov    es, ax
    mov    ss, ax
    mov    esp, BOOT_OFFSET

    mov    ax, 0600h  ; ┓
    mov    bx, 0700h  ; ┃
    mov    cx, 0      ; ┣ 清屏
    mov    dx, 0184fh ; ┃
    int    10h        ; ┛

    mov    ax, 0200h
    mov    bx, 0
    mov    dx, 0
    int    10h

    push   msg_boot
    push   msg_boot_len
    push   0000h
    call   r_disp_str
    add    esp, 6

    xor    ah, ah ; ┓
    xor    dl, dl ; ┣ 软驱复位
    int    13h    ; ┛

    push   es
    mov    ax, LOADER_SEG
    mov    es, ax
    mov    dx, 0000h         ; drive 0, head 0
    mov    cx, 02h           ; sector 2, track 0
    mov    bx, LOADER_OFFSET ; address
    mov    ah, 02h           ; service 2
    mov    al, LOADER_SECTOR ; nr of sectors
    int    13h
    pop    es

    push   es
    mov    ax, LOADER_SEG
    mov    es, ax,
    mov    ax, LOADER_OFFSET + LOADER_SIZE - LOADER_MAGIC_NR
    mov    di, ax
    mov    si, loader_mgic
    mov    cx, LOADER_MAGIC_NR
    cld
    CHECK_LOADER:
        cmp    cx, 0
        jz     LOADER_FOUNT
        dec    cx
        lodsb
        cmp    al, byte [es:di]
        jz     GOON_CHECK

        push   msg_loader_err
        push   msg_loader_err_len
        push   0200h
        call   r_disp_str
        add    esp, 6
        jmp    $

        GOON_CHECK:
            inc    di
            jmp    CHECK_LOADER

        LOADER_FOUNT:
    pop    es

    push   msg_ready
    push   msg_ready_len
    push   0102h
    call   r_disp_str
    add    esp, 6

    jmp    LOADER_SEG:LOADER_OFFSET

%include "io_rm.inc"

msg_boot:            db   "Booting ..."
msg_boot_len         equ  $ - msg_boot
msg_ready:           db   "Ready"
msg_ready_len        equ  $ - msg_ready
msg_loader_err:      db   "loader not found."
msg_loader_err_len   equ  $ - msg_loader_err

loader_mgic:    LOADER_MAGIC

times  510 - ($ - $$)  db  0
dw  0xaa55
