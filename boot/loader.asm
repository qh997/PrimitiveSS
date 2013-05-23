
%include "load.inc"

org    LOADER_OFF
jmp    START

START:
    mov    ax, cs
    mov    ds, ax
    mov    es, ax
    mov    ss, ax
    mov    esp, LOADER_OFF
    jmp    $

msg_boot:       db   "Loading ..."
msg_boot_len    equ  11

times  512 * LOADER_SZE - 4 - ($ - $$)  db  0
dd  081192h
