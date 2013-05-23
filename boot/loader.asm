
%include "defs.inc"

org    LOADER_OFF
jmp    START

START:
    mov    ax, cs
    mov    ds, ax
    mov    es, ax
    mov    ss, ax
    mov    esp, LOADER_OFF

    push   msg_load
    push   msg_load_len
    push   0300h
    call   disp_str1
    add    esp, 6

    jmp    $

%include "io.inc"

msg_load:       db   "Loading ..."
msg_load_len    equ  $ - msg_load

times  512 * LOADER_SZE - 4 - ($ - $$)  db  0
dd  081192h
