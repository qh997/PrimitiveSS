%include "boot/defs.inc"

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

    R_DISP_STR  0000h, msg_boot

    call   r_reset_floppy

    push   01h
    push   LOADER_SECTOR_NR
    push   LOADER_SEG
    push   LOADER_OFFSET
    call   r_read_sector
    add    esp, 8

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

        R_DISP_STR  0200h, msg_loader_err

        jmp    $

        GOON_CHECK:
            inc    di
            jmp    CHECK_LOADER

        LOADER_FOUNT:
    pop    es

    R_DISP_STR  0000h + msg_boot_len + 1, msg_ready

    jmp    LOADER_SEG:LOADER_OFFSET

%include "boot/io_rm.inc"

msg_boot:            db   "Booting ..."
msg_boot_len         equ  $ - msg_boot
msg_ready:           db   "OK"
msg_ready_len        equ  $ - msg_ready
msg_loader_err:      db   "Loader not found."
msg_loader_err_len   equ  $ - msg_loader_err

loader_mgic:    LOADER_MAGIC

times  510 - ($ - $$)  db  0
dw  0xaa55
