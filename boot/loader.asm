
%include "defs.inc"

org    LOADER_OFFSET
jmp    START

START:
    mov    ax, cs
    mov    ds, ax
    mov    es, ax
    mov    ss, ax
    mov    sp, LOADER_OFFSET

    push   msg_load_start
    push   msg_load_start_len
    push   0300h
    call   disp_str
    add    esp, 6

    jmp    $

%include "io.inc"

msg_load_start:     db   "Loading ..."
msg_load_start_len  equ  $ - msg_load_start

times  LOADER_SIZE - 5 - ($ - $$)  db  0
LOADER_MAGIC
